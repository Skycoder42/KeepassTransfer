TEMPLATE = app

QT += quick mvvmquick

TARGET = $${PROJECT_TARGET}-app

SOURCES += main.cpp

RESOURCES += \
	clientsquick.qrc

TRANSLATIONS += \
	kpt_client_quick_de.ts \
	kpt_client_quick_template.ts

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
