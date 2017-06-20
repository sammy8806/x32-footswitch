#-------------------------------------------------
#
# Project created by QtCreator 2017-06-11T19:03:06
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hsb_footcontroller
TEMPLATE = app

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../OSC/
LIBS += -L../OSC \
    -L../OSC/bin/debug \
    -L../OSC/bin/release \
    -L/home/steven/work/hsb_footcontroller/OSC/bin/debug/ \
    -L/home/steven/work/x32-footswitch/OSC/bin/debug/ \
    -losc

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    oscudpsocket.cpp \
    x32packetparser.cpp \
    x32Types/x32status.cpp \
    x32Types/x32configuserctrl.cpp \
    x32packetmanager.cpp \
    x32console.cpp \
    backgroundheartbeat.cpp \
    x32Types/mutegroup.cpp \
    x32Types/channel.cpp \
    x32Types/config.cpp

HEADERS += \
        mainwindow.h \
    oscudpsocket.h \
    x32packetparser.h \
    x32Types/x32status.h \
    x32Types/x32type.h \
    x32Types/x32configuserctrl.h \
    x32packetmanager.h \
    x32console.h \
    backgroundheartbeat.h \
    x32Types/mutegroup.h \
    x32Types/channel.h \
    x32Types/config.h

FORMS += mainwindow.ui
