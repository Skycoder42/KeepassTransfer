TEMPLATE = subdirs

SUBDIRS += \
	core \
	widgets \
	quick

widgets.depends += core
quick.depends += core
