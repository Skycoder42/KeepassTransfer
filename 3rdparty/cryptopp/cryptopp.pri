system_cryptopp {
	CONFIG += link_pkgconfig
	PKGCONFIG += libcrypto++
} else {
	OUT_ROOT = $$shadowed($$SRC_ROOT_DIR)

	win32:CONFIG(release, debug|release): LIBS += -L$$OUT_ROOT/3rdparty/cryptopp/release/ -lcryptopp
	else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_ROOT/3rdparty/cryptopp/debug/ -lcryptopp
	else:unix: LIBS += -L$$OUT_ROOT/3rdparty/cryptopp/ -lcryptopp

	INCLUDEPATH += $$OUT_ROOT/3rdparty
	DEPENDPATH += $$PWD/src

	win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_ROOT/3rdparty/cryptopp/release/libcryptopp.a
	else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_ROOT/3rdparty/cryptopp/debug/libcryptopp.a
	else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_ROOT/3rdparty/cryptopp/release/libcryptopp.lib
	else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_ROOT/3rdparty/cryptopp/debug/libcryptopp.lib
	else:unix: PRE_TARGETDEPS += $$OUT_ROOT/3rdparty/cryptopp/libcryptopp.a
}
