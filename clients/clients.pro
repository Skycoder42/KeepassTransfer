TEMPLATE = subdirs

SUBDIRS += \
	core \
	widgets \
	quick

no_widgets: SUBDIRS -= widgets
no_quick: SUBDIRS -= quick

widgets.depends += core
quick.depends += core

prepareRecursiveTarget(lrelease)
QMAKE_EXTRA_TARGETS += lrelease
