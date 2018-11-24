TEMPLATE = app

QT += widgets mvvmwidgets websockets concurrent
CONFIG += c++14

TARGET = $${PROJECT_TARGET}-client

HEADERS += \
	kptrootwizard.h \
	credentialseditpage.h \
	transferselectionpage.h

SOURCES += main.cpp \
	kptrootwizard.cpp \
	credentialseditpage.cpp \
	transferselectionpage.cpp

FORMS += \
	credentialseditpage.ui \
	transferselectionpage.ui

TRANSLATIONS += \
	kpt_client_widgets_de.ts \
	kpt_client_template.ts

DISTFILES += $$TRANSLATIONS \
	qpmx.json

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
