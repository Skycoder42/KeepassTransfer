TEMPLATE = subdirs

SUBDIRS += \
	core \
	widgets \
	quick

android: SUBDIRS -= widgets

widgets.depends += core
quick.depends += core
