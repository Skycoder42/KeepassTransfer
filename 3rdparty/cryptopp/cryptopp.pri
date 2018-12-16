system_cryptopp {
	CONFIG += link_pkgconfig
	PKGCONFIG += libcrypto++
} else {
	OUT_ROOT = $$shadowed($$SRC_ROOT_DIR)

	win32:CONFIG(release, debug|release): LIBS += -L$$OUT_ROOT/3rdparty/cryptopp/release/ -lqtcryptopp
	else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_ROOT/3rdparty/cryptopp/debug/ -lqtcryptopp
	else:unix: LIBS += -L$$OUT_ROOT/3rdparty/cryptopp/ -lqtcryptopp

	INCLUDEPATH += $$OUT_ROOT/include
	DEPENDPATH += $$PWD/src

	win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_ROOT/3rdparty/cryptopp/release/libqtcryptopp.a
	else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_ROOT/3rdparty/cryptopp/debug/libqtcryptopp.a
	else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_ROOT/3rdparty/cryptopp/release/qtcryptopp.lib
	else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_ROOT/3rdparty/cryptopp/debug/qtcryptopp.lib
	else:unix: PRE_TARGETDEPS += $$OUT_ROOT/3rdparty/cryptopp/libqtcryptopp.a

	# defines:
	win32:!win32-g++: DEFINES += CRYPTOPP_DISABLE_ASM
	equals(ANDROID_TARGET_ARCH, x86): DEFINES += CRYPTOPP_DISABLE_ASM
	win32:cross_compile: DEFINES += NO_OS_DEPENDENCE
}
