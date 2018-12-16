TEMPLATE = subdirs

SUBDIRS += \
	3rdparty \
	lib \
	webapp \
	clients

!no_server:!cross_compile: SUBDIRS += server
no_webapp: SUBDIRS -= webapp
wasm: SUBDIRS -= clients

lib.depends += 3rdparty
webapp.depends += 3rdparty lib
clients.depends += 3rdparty lib
server.depends += 3rdparty lib

prepareRecursiveTarget(lrelease)
QMAKE_EXTRA_TARGETS += lrelease

DISTFILES += \
	.qmake.conf

include(deploy/install.pri)

!isEmpty(PREFIX):!no_bundle_deploy {
	win32: DEPLOY_BINS = "$$INSTALL_BINS/$${PROJECT_TARGET}-client.exe" "$$INSTALL_BINS/$${PROJECT_TARGET}-server.exe"
	else:mac: DEPLOY_BINS = "$$PREFIX/$${PROJECT_NAME}.app" "$$PREFIX/$${APP_PREFIX}/MacOs/$${PROJECT_TARGET}-server"
	else:android: DEPLOY_BINS = "$$OUT_PWD/clients/quick/android-lib$${PROJECT_TARGET}-app.so-deployment-settings.json"
	DEPLOY_PLUGINS += servicebackends

	#!no_widgets: TS_DICTIONARIES += $$PWD/gui/widgets/kptransfer.tsdict
	#!no_quick: TS_DICTIONARIES += $$PWD/gui/quick/kptransfer.tsdict
}

include(deploy/deploy.pri)
