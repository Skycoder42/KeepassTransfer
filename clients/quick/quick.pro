TEMPLATE = app

QT += quick mvvmquick websockets concurrent
android: QT += androidextras
CONFIG(release, debug|release): CONFIG += qtquickcompiler

TARGET = $${PROJECT_TARGET}-app

QMAKE_TARGET_DESCRIPTION = "$$PROJECT_NAME Mobile App"
RC_ICONS += ../../icon/kpt.ico
ICON += ../../icon/kpt.icns

DEFINES += QTCSV_STATIC_LIB

HEADERS += \
	qrcodescanner.h \
	transferloader.h

SOURCES += main.cpp \
	qrcodescanner.cpp \
	transferloader.cpp

RESOURCES += \
	clientsquick.qrc

TRANSLATIONS += \
	kpt_client_quick_de.ts \
	kpt_client_quick_template.ts

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

DISTFILES += $$TRANSLATIONS \
	qpmx.json \
	android/AndroidManifest.xml \
	$$files(android/src/*, true) \
	$$files(android/res/*, true) \
	android/build.gradle \
	android/src/de/skycoder42/kpt/TransferAccessReceiver.java \
	android/src/de/skycoder42/kpt/TransferActionReceiver.java

include(../../deploy/install.pri)
target.path = $$INSTALL_BINS
qpmx_ts_target.path = $$INSTALL_TRANSLATIONS
INSTALLS += target qpmx_ts_target

# Link with core project
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -l$${PROJECT_TARGET}-core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -l$${PROJECT_TARGET}-core
else:unix: LIBS += -L$$OUT_PWD/../core/ -l$${PROJECT_TARGET}-core

INCLUDEPATH += $$PWD/../core $$OUT_PWD/../core
win32:CONFIG(release, debug|release): INCLUDEPATH += $$OUT_PWD/../core/release
else:win32:CONFIG(debug, debug|release): INCLUDEPATH += $$OUT_PWD/../core/debug
DEPENDPATH += $$PWD/../core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/lib$${PROJECT_TARGET}-core.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/lib$${PROJECT_TARGET}-core.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/$${PROJECT_TARGET}-core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/$${PROJECT_TARGET}-core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../core/lib$${PROJECT_TARGET}-core.a

packagesExist(libkpxcclient) {
	CONFIG += link_pkgconfig
	PKGCONFIG += libkpxcclient
	DEFINES += USE_KPXCCLIENT_LIB
}

!ReleaseBuild:!DebugBuild:!system(qpmx -d $$shell_quote($$_PRO_FILE_PWD_) --qmake-run init $$QPMX_EXTRA_OPTIONS $$shell_quote($$QMAKE_QMAKE) $$shell_quote($$OUT_PWD)): error(qpmx initialization failed. Check the compilation log for details.)
else: include($$OUT_PWD/qpmx_generated.pri)

include($$SRC_ROOT_DIR/lib/lib.pri)

contains(ANDROID_TARGET_ARCH,x86) {
	ANDROID_EXTRA_LIBS = \
		$$PWD/../../3rdparty/openssl/x86/libcrypto.so \
		$$PWD/../../3rdparty/openssl/x86/libssl.so
} else:contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
	ANDROID_EXTRA_LIBS = \
		$$PWD/../../3rdparty/openssl/armv7/libcrypto.so \
		$$PWD/../../3rdparty/openssl/armv7/libssl.so
}
