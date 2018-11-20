TEMPLATE = lib

CONFIG += staticlib
CONFIG -= qt
QT =

TARGET = qrcodegen

HEADERS += \
	src/cpp/BitBuffer.hpp \
	src/cpp/QrCode.hpp \
	src/cpp/QrSegment.hpp

SOURCES += \
	src/cpp/BitBuffer.cpp \
	src/cpp/QrCode.cpp \
	src/cpp/QrSegment.cpp

