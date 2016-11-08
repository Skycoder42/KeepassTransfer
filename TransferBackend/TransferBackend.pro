TEMPLATE = app

QT += core websockets
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = TransferBackend

include(../QBackgroundProcess/qbackgroundprocess.pri)

SOURCES += main.cpp \
    appserver.cpp \
    clienthandler.cpp

HEADERS += \
    appserver.h \
    clienthandler.h
