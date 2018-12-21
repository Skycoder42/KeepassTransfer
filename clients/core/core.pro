TEMPLATE = lib

QT += mvvmcore websockets concurrent
CONFIG += static

TARGET = $${PROJECT_TARGET}-core

DEFINES += QTCSV_STATIC_LIB

packagesExist(libkpxcclient) {
	CONFIG += link_pkgconfig
	PKGCONFIG += libkpxcclient
	DEFINES += USE_KPXCCLIENT_LIB

	HEADERS += kpxcclientimporter.h
	SOURCES += kpxcclientimporter.cpp
}

HEADERS += \
	kptclientapp.h \
	credentialseditviewmodel.h \
	transferselectionviewmodel.h \
	kptrootviewmodel.h \
	clienttransferservice.h \
	qrcodeconnectorviewmodel.h \
	qrclientencryptor.h \
	kptsettingsviewmodel.h \
	transferpreselectionentry.h \
	credentialsselectionviewmodel.h \
	passclientencryptor.h \
	passconnectorviewmodel.h

SOURCES += \
	kptclientapp.cpp \
	credentialseditviewmodel.cpp \
	transferselectionviewmodel.cpp \
	kptrootviewmodel.cpp \
	clienttransferservice.cpp \
	qrcodeconnectorviewmodel.cpp \
	qrclientencryptor.cpp \
	kptsettingsviewmodel.cpp \
	transferpreselectionentry.cpp \
	credentialsselectionviewmodel.cpp \
	passclientencryptor.cpp \
	passconnectorviewmodel.cpp

SETTINGS_DEFINITIONS += \
	settings.xml

RESOURCES += \
	clientscore.qrc

TRANSLATIONS += clients_core_de.ts \
	clients_core_template.ts

SETTINGS_TRANSLATIONS = gui_settings.xml

DISTFILES += $$TRANSLATIONS \
	qpmx.json

include($$SRC_ROOT_DIR/lib/lib.pri)
include($$SRC_ROOT_DIR/3rdparty/qtcsv/qtcsv.pri)

!ReleaseBuild:!DebugBuild:!system(qpmx -d $$shell_quote($$_PRO_FILE_PWD_) --qmake-run init $$QPMX_EXTRA_OPTIONS $$shell_quote($$QMAKE_QMAKE) $$shell_quote($$OUT_PWD)): error(qpmx initialization failed. Check the compilation log for details.)
else: include($$OUT_PWD/qpmx_generated.pri)
