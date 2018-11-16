TEMPLATE = subdirs

SUBDIRS += \
	3rdparty \
	webapp

webapp.depends += 3rdparty

DISTFILES += \
	.qmake.conf
