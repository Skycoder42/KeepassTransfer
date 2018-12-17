TEMPLATE = app

QT += widgets mvvmwidgets websockets concurrent
CONFIG += c++14

mac: TARGET = "$$PROJECT_NAME"
else: TARGET = $${PROJECT_TARGET}-client

QMAKE_TARGET_DESCRIPTION = "$$PROJECT_NAME Desktop Client"
RC_ICONS += ../../icon/kpt.ico
ICON += ../../icon/kpt.icns

DEFINES += QTCSV_STATIC_LIB

HEADERS += \
	kptrootwizard.h \
	credentialseditpage.h \
	transferselectionpage.h \
	qrcodeconnectorpage.h \
	transferpreselectionedit.h \
	credentialsselectionpage.h \
	passconnectorpage.h

SOURCES += main.cpp \
	kptrootwizard.cpp \
	credentialseditpage.cpp \
	transferselectionpage.cpp \
	qrcodeconnectorpage.cpp \
	transferpreselectionedit.cpp \
	credentialsselectionpage.cpp \
	passconnectorpage.cpp

FORMS += \
	credentialseditpage.ui \
	transferselectionpage.ui \
	qrcodeconnectorpage.ui \
	credentialsselectionpage.ui \
	passconnectorpage.ui

RESOURCES += \
	clientswidgets.qrc

TRANSLATIONS += \
	kpt_client_widgets_de.ts \
	kpt_client_widgets_template.ts

DISTFILES += $$TRANSLATIONS \
	qpmx.json \
	de.skycoder42.kptransfer.desktop

include(../../deploy/install.pri)
linux {
	create_icons.target = create_icons
	create_icons.commands += $$shell_path($$PWD/../../deploy/create-icons.sh) de.skycoder42.kptransfer $$shell_path($$PWD/../../icon/pngs/kpt) $$ICON_SIZES
	QMAKE_EXTRA_TARGETS += create_icons

	desktop_install.files = de.skycoder42.kptransfer.desktop
	desktop_install.path = $$INSTALL_SHARE/applications/
	install_icons.files += $$shadowed(icon_export/hicolor)
	install_icons.path = $$INSTALL_SHARE/icons/
	install_icons.CONFIG += no_check_exist
	install_icons.depends += create_icons
	INSTALLS += desktop_install install_icons
}
mac: target.path = $$INSTALL_APPS
else: target.path = $$INSTALL_BINS
qpmx_ts_target.path = $$INSTALL_TRANSLATIONS
INSTALLS += target qpmx_ts_target

# Link with core project
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -l$${PROJECT_TARGET}-core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -l$${PROJECT_TARGET}-core
else:unix: LIBS += -L$$OUT_PWD/../core/ -l$${PROJECT_TARGET}-core

INCLUDEPATH += $$PWD/../core  $$OUT_PWD/../core
win32:CONFIG(release, debug|release): INCLUDEPATH += $$OUT_PWD/../core/release
else:win32:CONFIG(debug, debug|release): INCLUDEPATH += $$OUT_PWD/../core/debug
DEPENDPATH += $$PWD/../core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/lib$${PROJECT_TARGET}-core.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/lib$${PROJECT_TARGET}-core.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/$${PROJECT_TARGET}-core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/$${PROJECT_TARGET}-core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../core/lib$${PROJECT_TARGET}-core.a

!ReleaseBuild:!DebugBuild:!system(qpmx -d $$shell_quote($$_PRO_FILE_PWD_) --qmake-run init $$QPMX_EXTRA_OPTIONS $$shell_quote($$QMAKE_QMAKE) $$shell_quote($$OUT_PWD)): error(qpmx initialization failed. Check the compilation log for details.)
else: include($$OUT_PWD/qpmx_generated.pri)

include($$SRC_ROOT_DIR/lib/lib.pri)
