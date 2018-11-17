TEMPLATE = app

TARGET = $${PROJECT_TARGET}-webapp

QT += quick quickcontrols2 websockets
CONFIG += qtquickcompiler

SOURCES += \
	main.cpp \
    serverconnector.cpp

RESOURCES += qml.qrc

include($$SRC_ROOT_DIR/3rdparty/qzxing/qzxing.pri)
include($$SRC_ROOT_DIR/3rdparty/cryptopp/cryptopp.pri)
include($$SRC_ROOT_DIR/lib/lib.pri)

HEADERS += \
    serverconnector.h
