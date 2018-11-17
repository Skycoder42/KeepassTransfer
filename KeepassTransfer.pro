TEMPLATE = subdirs

SUBDIRS += \
	3rdparty \
	webapp \
    clients

!cross_compile: SUBDIRS += server

webapp.depends += 3rdparty
server.depends += 3rdparty

DISTFILES += \
	.qmake.conf
