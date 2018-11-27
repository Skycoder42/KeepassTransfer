TEMPLATE = aux

wasm {
	QMAKE_CXXFLAGS_RELEASE += "-DNDEBUG"

	cryptopp_target.target = static
	cryptopp_target.commands += \
		test -f GNUmakefile || $$QMAKE_COPY_DIR $$shell_path($$PWD/src/*) .
	cryptopp_target.commands += $$escape_expand(\\n\\t)+emmake $(MAKE) -f GNUmakefile static CFLAGS=\"$(CFLAGS)\" CXXFLAGS=\"$(CXXFLAGS)\" LFLAGS=\"$(LFLAGS)\"
	QMAKE_EXTRA_TARGETS += cryptopp_target
	PRE_TARGETDEPS += static
} else:android {
	equals(ANDROID_TARGET_ARCH, armeabi-v7a): ANDROID_TARGET_ARCH_SHORT = armv7
	else: ANDROID_TARGET_ARCH_SHORT = $$ANDROID_TARGET_ARCH
	# download the precompiled binaries
	download_target.target = cryptopp.tar.xz
	download_target.commands = curl -Lo $$shell_path($$OUT_PWD/cryptopp.tar.xz) \
		$$shell_quote(https://github.com/Skycoder42/ci-builds/releases/download/cryptopp_7_0_0/cryptopp_7_0_0_android_$${ANDROID_TARGET_ARCH_SHORT}.tar.xz)

	unpack_target.target = unpack
	unpack_target.depends += cryptopp.tar.xz
	unpack_target.commands += tar -xf $$shell_path($$OUT_PWD/cryptopp.tar.xz) -C $$shell_path($$OUT_PWD) \
		$$escape_expand(\\n\\t)$$QMAKE_COPY $$shell_path($$OUT_PWD/cryptopp/include/cryptopp/*.h) $$shell_path($$OUT_PWD/) \
		$$escape_expand(\\n\\t)$$QMAKE_COPY $$shell_path($$OUT_PWD/cryptopp/lib/*.a) $$shell_path($$OUT_PWD/) \
		$$escape_expand(\\n\\t)touch unpack

	QMAKE_EXTRA_TARGETS += download_target unpack_target
	PRE_TARGETDEPS += unpack
}
