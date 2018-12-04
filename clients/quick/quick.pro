TEMPLATE = app

QT += quick mvvmquick websockets concurrent
android: QT += androidextras
CONFIG(release, debug|release): CONFIG += qtquickcompiler

TARGET = $${PROJECT_TARGET}-app

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

# Link with core project
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -l$${PROJECT_TARGET}-core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -l$${PROJECT_TARGET}-core
else:unix: LIBS += -L$$OUT_PWD/../core/ -l$${PROJECT_TARGET}-core

INCLUDEPATH += $$PWD/../core $$OUT_PWD/../core
DEPENDPATH += $$PWD/../core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/lib$${PROJECT_TARGET}-core.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/lib$${PROJECT_TARGET}-core.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/lib$${PROJECT_TARGET}-core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/lib$${PROJECT_TARGET}-core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../core/lib$${PROJECT_TARGET}-core.a

!ReleaseBuild:!DebugBuild:!system(qpmx -d $$shell_quote($$_PRO_FILE_PWD_) --qmake-run init $$QPMX_EXTRA_OPTIONS $$shell_quote($$QMAKE_QMAKE) $$shell_quote($$OUT_PWD)): error(qpmx initialization failed. Check the compilation log for details.)
else: include($$OUT_PWD/qpmx_generated.pri)

include($$SRC_ROOT_DIR/lib/lib.pri)

HEADERS += \
	qrcodescanner.h \
	transferloader.h
