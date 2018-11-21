TEMPLATE = lib

QT += mvvmcore
CONFIG += static

TARGET = $${PROJECT_TARGET}-core

HEADERS += \
	mainviewmodel.h \
    kptclientapp.h

SOURCES += \
	mainviewmodel.cpp \
    kptclientapp.cpp

RESOURCES += \
	clientscore.qrc

TRANSLATIONS += clients_core_de.ts \
	clients_core_template.ts

DISTFILES += $$TRANSLATIONS
QTMVVM_TS_SETTINGS = settings.xml
_never_true_condition: SOURCES += $$files($$PWD/.ts-dummy/*)
# Uncomment the following line to automatically generated and update settings translations when building
#PRE_TARGETDEPS += qtmvvm-tsgen
