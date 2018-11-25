TEMPLATE = lib

QT = core
CONFIG += staticlib

TARGET = $$PROJECT_TARGET

HEADERS += \
	kptlib.h \
	messages/appidentmessage.h \
	messages/serveridentmessage.h \
	messages/errormessage.h \
	messages/clienttransfermessage.h \
	messages/servertransfermessage.h \
	messages/appokmessage.h \
	messages/serverokmessage.h \
	dataencryptor.h \
    encrypteddata.h \
    credential.h \
    qrdata.h

SOURCES += \
	kptlib.cpp \
	messages/appidentmessage.cpp \
	messages/serveridentmessage.cpp \
	messages/errormessage.cpp \
	messages/clienttransfermessage.cpp \
	messages/servertransfermessage.cpp \
	messages/appokmessage.cpp \
	messages/serverokmessage.cpp \
	dataencryptor.cpp \
    encrypteddata.cpp \
    credential.cpp \
    qrdata.cpp

DISTFILES += \
	protocol.qmodel \
	qpmx.json

include($$SRC_ROOT_DIR/3rdparty/cryptopp/cryptopp.pri)

!ReleaseBuild:!DebugBuild:!system(qpmx -d $$shell_quote($$_PRO_FILE_PWD_) --qmake-run init $$QPMX_EXTRA_OPTIONS $$shell_quote($$QMAKE_QMAKE) $$shell_quote($$OUT_PWD)): error(qpmx initialization failed. Check the compilation log for details.)
else: include($$OUT_PWD/qpmx_generated.pri)
