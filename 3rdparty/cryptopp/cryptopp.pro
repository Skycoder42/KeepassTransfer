TEMPLATE = aux

QMAKE_CXXFLAGS_RELEASE += "-DNDEBUG"

cryptopp_target.target = static
cryptopp_target.commands +=  \
	test -f GNUmakefile || $$QMAKE_COPY_DIR $$shell_path($$PWD/src/*) . \
	$$escape_expand(\\n\\t)+emmake $(MAKE) -f GNUmakefile static CFLAGS=\"$(CFLAGS)\" CXXFLAGS=\"$(CXXFLAGS)\" LFLAGS=\"$(LFLAGS)\"
QMAKE_EXTRA_TARGETS += cryptopp_target
PRE_TARGETDEPS += static
