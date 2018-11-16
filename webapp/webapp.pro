TEMPLATE = app

TARGET = $${PROJECT_TARGET}_webapp

QT += quick quickcontrols2
CONFIG += qtquickcompiler

SOURCES += \
	main.cpp \
    cryptopp_test.cpp

RESOURCES += qml.qrc

include($$SRC_ROOT_DIR/3rdparty/qzxing/qzxing.pri)
include($$SRC_ROOT_DIR/3rdparty/cryptopp/cryptopp.pri)
