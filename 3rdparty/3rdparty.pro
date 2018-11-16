TEMPLATE = subdirs

SUBDIRS += \
	cryptopp \
	qzxing

system_cryptopp: SUBDIRS -= cryptopp

DISTFILES += \
	cryptopp/cryptopp.pri \
	qzxing/qzxing.pri
