TEMPLATE = subdirs

SUBDIRS += \
	3rdparty \
	lib \
	webapp \
	clients

!cross_compile: SUBDIRS += server

lib.depends += 3rdparty
webapp.depends += 3rdparty lib
clients.depends += 3rdparty lib
server.depends += 3rdparty lib

DISTFILES += \
	.qmake.conf
