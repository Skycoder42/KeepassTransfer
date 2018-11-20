TEMPLATE = subdirs

SUBDIRS += \
	cryptopp \
	QR-Code-generator

system_cryptopp: SUBDIRS -= cryptopp

DISTFILES += \
	cryptopp/cryptopp.pri \
	QR-Code-generator/QR-Code-generator.pri
