TEMPLATE = app

TARGET = $${PROJECT_TARGET}_webapp

QT += quick
CONFIG += qtquickcompiler

SOURCES += \
	main.cpp

RESOURCES += qml.qrc
