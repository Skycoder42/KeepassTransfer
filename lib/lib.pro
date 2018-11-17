TEMPLATE = lib

QT = core
CONFIG += staticlib

TARGET = $$PROJECT_TARGET

HEADERS += \
	kptlib.h \
	messages/appidentmessage.h \
    messages/serveridentmessage.h \
    messages/errormessage.h \
    messages/clienttransfermessage.h \
    messages/servertransfermessage.h \
    messages/appokmessage.h \
    messages/serverokmessage.h

SOURCES += \
	kptlib.cpp \
	messages/appidentmessage.cpp \
    messages/serveridentmessage.cpp \
    messages/errormessage.cpp \
    messages/clienttransfermessage.cpp \
    messages/servertransfermessage.cpp \
    messages/appokmessage.cpp \
    messages/serverokmessage.cpp

DISTFILES += \
	protocol.qmodel
