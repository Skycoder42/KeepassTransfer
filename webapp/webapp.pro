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

HEADERS += \
	serverconnector.h \
	iencoder.h \
	qrencoder.h \
	browserstorage.h \
    qrimageprovider.h

SOURCES += \
	main.cpp \
	serverconnector.cpp \
	iencoder.cpp \
	qrencoder.cpp \
    qrimageprovider.cpp

RESOURCES += \
	webapp.qrc

wasm: SOURCES += browserstorage_wasm.cpp
else: SOURCES += browserstorage.cpp

include($$SRC_ROOT_DIR/lib/lib.pri)
include($$SRC_ROOT_DIR/3rdparty/QR-Code-generator/QR-Code-generator.pri)

html_install.files += $${TARGET}.html \
	$${TARGET}.js \
	$${TARGET}.wasm \
	qtloader.js \
	qtlogo.svg
html_install.path = www
INSTALLS += html_install
