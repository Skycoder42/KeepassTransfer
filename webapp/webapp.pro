TEMPLATE = app

TARGET = $${PROJECT_TARGET}-webapp

QT += quick websockets svg quickcontrols2
CONFIG += qtquickcompiler

QTPLUGIN.imageformats = qsvg

# hack to exclude QML plugins:
ADDED_IMPORTS += \
	qtquickcontrols2universalstyleplugin \
	qtquickcontrols2fusionstyleplugin \
	qtquickcontrols2imaginestyleplugin

SOURCES += \
	main.cpp \
	serverconnector.cpp

RESOURCES += qml.qrc

include($$SRC_ROOT_DIR/3rdparty/qzxing/qzxing.pri)
include($$SRC_ROOT_DIR/3rdparty/cryptopp/cryptopp.pri)
include($$SRC_ROOT_DIR/lib/lib.pri)

HEADERS += \
	serverconnector.h

html_install.files += $${TARGET}.html \
	$${TARGET}.js \
	$${TARGET}.wasm \
	qtloader.js \
	qtlogo.svg
html_install.path = www
INSTALLS += html_install
