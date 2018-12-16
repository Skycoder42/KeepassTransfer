TEMPLATE = subdirs

SUBDIRS += \
	3rdparty \
	lib \
	webapp \
	clients

!cross_compile: SUBDIRS += server
wasm: SUBDIRS -= clients

lib.depends += 3rdparty
webapp.depends += 3rdparty lib
clients.depends += 3rdparty lib
server.depends += 3rdparty lib

DISTFILES += \
	.qmake.conf

include(install.pri)
!no_bundle_deploy:mac {
	install.commands += cp -pPRfv "$(INSTALL_ROOT)$$INSTALL_APPS/" "$(INSTALL_ROOT)$${PREFIX}/"$$escape_expand(\n\t) \
		rm -rf "$(INSTALL_ROOT)$$INSTALL_APPS"$$escape_expand(\n\t) \
		install_name_tool -change lib$${PROJECT_TARGET}.3.dylib @rpath/lib$${PROJECT_TARGET}.3.dylib "$(INSTALL_ROOT)$${INSTALL_BINS}/$${PROJECT_TARGET}"$$escape_expand(\n\t) \
		install_name_tool -change lib$${PROJECT_TARGET}.3.dylib @rpath/lib$${PROJECT_TARGET}.3.dylib "$(INSTALL_ROOT)$${INSTALL_BINS}/$${PROJECT_TARGET}d"$$escape_expand(\n\t)
	QMAKE_EXTRA_TARGETS += install
}

!isEmpty(PREFIX):!no_bundle_deploy {
	win32: DEPLOY_BINS = "$$INSTALL_BINS/$${PROJECT_TARGET}.exe" "$$INSTALL_BINS/$${PROJECT_TARGET}d.exe"
	else:mac: DEPLOY_BINS = "$$PREFIX/$${PROJECT_NAME}.app" "$$PREFIX/$${APP_PREFIX}/MacOs/syremd"
	else:android: DEPLOY_BINS = "$$OUT_PWD/gui/quick/android-libkptransfer-app.so-deployment-settings.json"
	systemd_service: DEPLOY_PLUGINS += servicebackends

	#!no_widgets: TS_DICTIONARIES += $$PWD/gui/widgets/kptransfer.tsdict
	#!no_quick: TS_DICTIONARIES += $$PWD/gui/quick/kptransfer.tsdict
}

include(deploy/deploy.pri)
