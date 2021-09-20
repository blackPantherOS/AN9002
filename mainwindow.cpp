#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "multimeter.h"

#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    deviceDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    deviceDiscoveryAgent->setLowEnergyDiscoveryTimeout(5000);

    QObject::connect(deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, [this](const QBluetoothDeviceInfo &device) {
        qInfo() << "DEVICE DISCOVERED:" << device.address() << device.serviceUuids();

        if (!(device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration))
            return;
	//DEVICE DISCOVERED: "FC:58:FA:4C:77:D6" ("{00001800-0000-1000-8000-00805f9b34fb}", "{00001801-0000-1000-8000-00805f9b34fb}", "{0000180a-0000-1000-8000-00805f9b34fb}", "{0000fff0-0000-1000-8000-00805f9b34fb}", "{f000ffc0-0451-4000-b000-000000000000}")

        if (!device.serviceUuids().contains(QBluetoothUuid(QString("00001800-0000-1000-8000-00805f9b34fb"))))
	//  if (!device.serviceUuids().contains(QBluetoothUuid(QString("0bd51666-e7cb-469b-8e4d-2742f1ba77cc"))))
            return;

        detectedDevices << device;
        new QListWidgetItem(device.name() + " [" + device.address().toString() + "]", ui->listWidget);
    });

    QObject::connect(deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &MainWindow::scanEnded);
    QObject::connect(deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled, this, &MainWindow::scanEnded);
    QObject::connect(ui->scanButton, &QPushButton::clicked, this, &MainWindow::scan);

    QObject::connect(ui->listWidget, &QListWidget::doubleClicked, [this](const QModelIndex &index) {
        QBluetoothDeviceInfo device = detectedDevices.at(index.row());

        foreach (Multimeter *multimeter, multimeters) {
            if (multimeter->getDeviceInfo() == device) {
                multimeter->activateWindow();
                multimeter->show();
                return;
            }
        }

        Multimeter *multimeter = new Multimeter(device);
        multimeters << multimeter;
        multimeter->show();
        
	qInfo() << "Connect:" << multimeter;
        
        QObject::connect(multimeter, &Multimeter::destroyed, [this]() {
            Multimeter *multimeter = (Multimeter *) sender();
            multimeters.removeAll(multimeter);
        });
    });

    scan();
}

void MainWindow::scan()
{
    detectedDevices.clear();
    ui->listWidget->clear();
    ui->scanButton->setEnabled(false);
    ui->scanButton->setText("Scanning ...");
    deviceDiscoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void MainWindow::scanEnded() {
    ui->scanButton->setEnabled(true);
    ui->scanButton->setText("Scan for BLE multimeters");
}

MainWindow::~MainWindow()
{
    delete ui;
}
