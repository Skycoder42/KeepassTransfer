TEMPLATE = app

QT = core service websockets
CONFIG += c++14 console
CONFIG -= app_bundle

TARGET = $${PROJECT_TARGET}-server

HEADERS += \
	kptservice.h

SOURCES += \
	main.cpp \
	kptservice.cpp

DISTFILES += \
	kptservice.service \
	kptservice.socket \
	kptservice-install.bat \
	de.skycoder42.kptservice.plist
