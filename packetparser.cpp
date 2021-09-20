#include <QDebug>
#include <QtEndian>
#include "packetparser.h"

struct PacketV2
{
    uint32_t serial;
    uint8_t mainMode;
    uint8_t mainRange;
    uint16_t mainValue;
    uint8_t subMode;
    uint8_t subRange;
    uint16_t subValue;
//    uint8_t barStatus;
//    uint8_t barValue;
    uint8_t iconStatus[3];
    uint8_t checksum;
} __attribute__((packed));

PacketParser::PacketParser(PacketVersion version, QObject *parent) :
    QObject(parent),
  packetVersion(version)
{
}

bool PacketParser::parseV2(const QByteArray &data)
{
    const struct PacketV2 *packet;

    if (sizeof(*packet) != data.length())
	 qInfo() << "DATA1 :"<< sizeof(*packet) << "DATA2" << data.length() ; 
        return false;

     packet = (const struct PacketV2 *) data.constData();
     qInfo() << "PACKET: " << packet ; 

    uint8_t checksum = 0xf2;

    for (int i = 0; i < data.length() - 1; i++)
        checksum ^= data.at(i);

    qInfo() << "CHECKSUM1 :" << checksum ; 
    if (checksum != packet->checksum)
	     qInfo() << "CHECKSUM2 :" << packet->checksum ; 
        //    return false;

    mainMode = (Mode) (packet->mainMode & 0x1f);
    subMode = (Mode) packet->subMode;
//    barValue = packet->barValue;

    mainValue = qFromBigEndian(packet->mainValue) | ((packet->mainMode >> 6) << 16);
    mainRangeFlags = (MainRangeFlags) (packet->mainRange >> 4);
    mainScale = packet->mainRange & 0xf;

    qInfo() << QString::asprintf("mainRange: %02x", packet->mainRange & 0xf) << "value" << mainValue;
//    qInfo() << QString::asprintf("mainRange: %02x", packet->mainRange & 0xf) << "value" << mainValue;

    subValue = qFromBigEndian(packet->subValue);

//    barFlags = (BarFlags) packet->barStatus;
//    barScale = (BarScale) (packet->barStatus & 0x3);
    currentIcons = (Icons)
            ((packet->iconStatus[0]) |
             (packet->iconStatus[1] << 8) |
             (packet->iconStatus[2] << 16));

    return true;
}

bool PacketParser::parse(const QByteArray &data)
{
    if (packetVersion == PacketVersion2)
        return parseV2(data);

     qInfo() << "PacketVersions :" << packetVersion << PacketVersion2 ; 
    return false;
}
