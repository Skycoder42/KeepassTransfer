TEMPLATE = app

TARGET = $${PROJECT_TARGET}-webapp

QT += quick websockets svg quickcontrols2
CONFIG(release, debug|release): CONFIG += qtquickcompiler

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
	qrimageprovider.h \
	emjsconnector.h

SOURCES += \
	main.cpp \
	serverconnector.cpp \
	iencoder.cpp \
	qrencoder.cpp \
	qrimageprovider.cpp

RESOURCES += \
	webapp.qrc

wasm {
	HEADERS += \
		qwasmsettings.h

	SOURCES += \
		qwasmsettings.cpp \
		emjsconnector_wasm.cpp
} else {
	SOURCES += \
		emjsconnector.cpp
}


include($$SRC_ROOT_DIR/lib/lib.pri)
include($$SRC_ROOT_DIR/3rdparty/QR-Code-generator/QR-Code-generator.pri)

html_install.files += $${TARGET}.html \
	$${TARGET}.js \
	$${TARGET}.wasm \
	qtloader.js \
	qtlogo.svg
html_install.path = www
INSTALLS += html_install
