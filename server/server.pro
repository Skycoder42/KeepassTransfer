TEMPLATE = app

QT = core service websockets
CONFIG += c++17	console
CONFIG -= app_bundle c++14

TARGET = $${PROJECT_TARGET}-server

QMAKE_TARGET_DESCRIPTION = "$$PROJECT_NAME Server"
RC_ICONS += ../icon/kpt.ico

HEADERS += \
	kptservice.h \
	transferserver.h

SOURCES += \
	main.cpp \
	kptservice.cpp \
	transferserver.cpp

DISTFILES += \
	kptserver.service.in \
	kptserver.socket \
	kptserver-install.bat \
	de.skycoder42.kptransfer.server.plist

# install
include(../deploy/install.pri)

win32 {
	install_service.files += $$PWD/kptserver-install.bat
	install_service.path = $$INSTALL_BINS
} else:mac {
	install_service.files += $$PWD/de.skycoder42.kptransfer.server.plist
	install_service.path = $$INSTALL_SHARE/LaunchAgents
} else: {
	create_service.target = kptserver.service
	create_service.depends += $$PWD/kptserver.service.in
	create_service.commands += sed "s:%{INSTALL_BINS}:$$INSTALL_BINS:g" $$PWD/kptserver.service.in > kptserver.service

	QMAKE_EXTRA_TARGETS += create_service
	PRE_TARGETDEPS += kptserver.service

	install_service.files += $$OUT_PWD/kptserver.service kptserver.socket
	install_service.CONFIG += no_check_exist
	install_service.path = $$INSTALL_LIBS/systemd/system/
}

target.path = $$INSTALL_BINS
qpmx_ts_target.path = $$INSTALL_TRANSLATIONS
INSTALLS += target install_service qpmx_ts_target

include($$SRC_ROOT_DIR/lib/lib.pri)

!ReleaseBuild:!DebugBuild:!system(qpmx -d $$shell_quote($$_PRO_FILE_PWD_) --qmake-run init $$QPMX_EXTRA_OPTIONS $$shell_quote($$QMAKE_QMAKE) $$shell_quote($$OUT_PWD)): error(qpmx initialization failed. Check the compilation log for details.)
else: include($$OUT_PWD/qpmx_generated.pri)
