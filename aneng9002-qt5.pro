#-------------------------------------------------
#
# Project created by QtCreator 2018-08-10T20:42:29
#
#-------------------------------------------------

QT       += core gui bluetooth svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = an9002-qt5
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#DEFINES += QT_NO_DEBUG_OUTPUT
#DEFINES += QT_DEBUG_OUTPUT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += debug

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    multimeter.cpp \
    display.cpp \
    packetparser.cpp

HEADERS += \
    mainwindow.h \
    multimeter.h \
    display.h \
    packetparser.h

FORMS += \
    mainwindow.ui \
    multimeter.ui

RESOURCES += \
    assets.qrc
