TEMPLATE = app

QT = core service websockets
CONFIG -= app_bundle c++14
CONFIG += c++17	console

TARGET = $${PROJECT_TARGET}-server

HEADERS += \
	kptservice.h \
	transferserver.h

SOURCES += \
	main.cpp \
	kptservice.cpp \
	transferserver.cpp

DISTFILES += \
	kptservice.service \
	kptservice.socket \
	kptservice-install.bat \
	de.skycoder42.kptservice.plist

# install
include(../install.pri)
target.path = $$INSTALL_BINS
qpmx_ts_target.path = $$INSTALL_TRANSLATIONS
INSTALLS += target qpmx_ts_target

include($$SRC_ROOT_DIR/lib/lib.pri)

!ReleaseBuild:!DebugBuild:!system(qpmx -d $$shell_quote($$_PRO_FILE_PWD_) --qmake-run init $$QPMX_EXTRA_OPTIONS $$shell_quote($$QMAKE_QMAKE) $$shell_quote($$OUT_PWD)): error(qpmx initialization failed. Check the compilation log for details.)
else: include($$OUT_PWD/qpmx_generated.pri)
