TEMPLATE = lib

QT += mvvmcore
CONFIG += static

TARGET = $${PROJECT_TARGET}-core

HEADERS += \
	kptclientapp.h \
	credentialseditviewmodel.h \
    encryptionservice.h \
    transferselectionviewmodel.h \
    kptrootviewmodel.h

SOURCES += \
	kptclientapp.cpp \
	credentialseditviewmodel.cpp \
    encryptionservice.cpp \
    transferselectionviewmodel.cpp \
    kptrootviewmodel.cpp

RESOURCES += \
	clientscore.qrc

TRANSLATIONS += clients_core_de.ts \
	clients_core_template.ts

SETTINGS_TRANSLATIONS = settings.xml

DISTFILES += $$TRANSLATIONS \
	qpmx.json

include($$SRC_ROOT_DIR/lib/lib.pri)

!ReleaseBuild:!DebugBuild:!system(qpmx -d $$shell_quote($$_PRO_FILE_PWD_) --qmake-run init $$QPMX_EXTRA_OPTIONS $$shell_quote($$QMAKE_QMAKE) $$shell_quote($$OUT_PWD)): error(qpmx initialization failed. Check the compilation log for details.)
else: include($$OUT_PWD/qpmx_generated.pri)
