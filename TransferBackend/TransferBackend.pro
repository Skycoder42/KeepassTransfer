TEMPLATE = app

QT += core websockets
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = KeepassTransferBackend
VERSION = 1.0.0

DEFINES += "TARGET=\\\"$$TARGET\\\""
DEFINES += "VERSION=\\\"$$VERSION\\\""

win32 {
	QMAKE_TARGET_COMPANY = "Skycoder42"
	QMAKE_TARGET_PRODUCT = "Keepass Transfer Backend"
	QMAKE_TARGET_DESCRIPTION = $$QMAKE_TARGET_PRODUCT
	DEFINES += "COMPANY=\"\\\"$$QMAKE_TARGET_COMPANY\\\"\""
	DEFINES += "DISPLAY_NAME=\"\\\"$$QMAKE_TARGET_PRODUCT\\\"\""
} else:mac {
	DEFINES += "COMPANY=\"\\\"Skycoder42\\\"\""
	DEFINES += "DISPLAY_NAME=\"\\\"Keepass Transfer Backend\\\"\""
} else:unix {
	DEFINES += "COMPANY=\"\\\"Skycoder42\\\"\""
	DEFINES += "DISPLAY_NAME=\"\\\"Keepass Transfer Backend\\\"\""
	QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN/lib\''
}

include(../QtBackgroundProcess/qtbackgroundprocess.pri)

SOURCES += main.cpp \
    appserver.cpp \
    clienthandler.cpp

HEADERS += \
    appserver.h \
    clienthandler.h
