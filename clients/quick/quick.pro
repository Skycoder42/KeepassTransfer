TEMPLATE = app

QT += quick mvvmquick websockets concurrent
CONFIG += qtquickcompiler

TARGET = $${PROJECT_TARGET}-app

SOURCES += main.cpp

RESOURCES += \
	clientsquick.qrc

TRANSLATIONS += \
	kpt_client_quick_de.ts \
	kpt_client_quick_template.ts

DISTFILES += $$TRANSLATIONS \
	qpmx.json \
	android/AndroidManifest.xml \
	android/res/values/libs.xml \
	android/build.gradle

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

# Link with core project
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -l$${PROJECT_TARGET}-core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -l$${PROJECT_TARGET}-core
else:unix: LIBS += -L$$OUT_PWD/../core/ -l$${PROJECT_TARGET}-core

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/lib$${PROJECT_TARGET}-core.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/lib$${PROJECT_TARGET}-core.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/lib$${PROJECT_TARGET}-core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/lib$${PROJECT_TARGET}-core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../core/lib$${PROJECT_TARGET}-core.a

!ReleaseBuild:!DebugBuild:!system(qpmx -d $$shell_quote($$_PRO_FILE_PWD_) --qmake-run init $$QPMX_EXTRA_OPTIONS $$shell_quote($$QMAKE_QMAKE) $$shell_quote($$OUT_PWD)): error(qpmx initialization failed. Check the compilation log for details.)
else: include($$OUT_PWD/qpmx_generated.pri)

include($$SRC_ROOT_DIR/lib/lib.pri)

# link with openssl (android)
android {
	equals(ANDROID_TARGET_ARCH, armeabi-v7a): ANDROID_TARGET_ARCH_SHORT = armv7
	else: ANDROID_TARGET_ARCH_SHORT = $$ANDROID_TARGET_ARCH
	# download the precompiled binaries
	download_openssl.target = openssl.tar.xz
	download_openssl.commands = curl -Lo $$shell_path($$OUT_PWD/openssl.tar.xz) \
		$$shell_quote(https://github.com/Skycoder42/ci-builds/releases/download/1.0.2p/openssl_1.0.2p_android_$${ANDROID_TARGET_ARCH_SHORT}.tar.xz)

	unpack_openssl.target = unpack
	unpack_openssl.depends += openssl.tar.xz
	unpack_openssl.commands += tar -xf $$shell_path($$OUT_PWD/openssl.tar.xz) -C $$shell_path($$OUT_PWD) \
		$$escape_expand(\\n\\t)touch unpack

	QMAKE_EXTRA_TARGETS += download_openssl unpack_openssl
	PRE_TARGETDEPS += unpack

	LIBS += -L$$OUT_PWD/openssl -lcrypto -lssl
	ANDROID_EXTRA_LIBS += \
		$$OUT_PWD/openssl/libcrypto.so \
		$$OUT_PWD/openssl/libssl.so
}
