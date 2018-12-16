# installing
isEmpty(PREFIX) {
	PREFIX = $$[QT_INSTALL_PREFIX]
	isEmpty(INSTALL_BINS): INSTALL_BINS = $$[QT_INSTALL_BINS]
	isEmpty(INSTALL_APPS): INSTALL_APPS = $$INSTALL_BINS
	isEmpty(INSTALL_LIBS): INSTALL_LIBS = $$[QT_INSTALL_LIBS]
	isEmpty(INSTALL_HEADERS): INSTALL_HEADERS = $$[QT_INSTALL_HEADERS]
	isEmpty(INSTALL_PLUGINS): INSTALL_PLUGINS = $$[QT_INSTALL_PLUGINS]
	isEmpty(INSTALL_TRANSLATIONS): INSTALL_TRANSLATIONS = $$[QT_INSTALL_TRANSLATIONS]
	isEmpty(INSTALL_SHARE): INSTALL_SHARE = $${PREFIX}/share
	CONFIG += no_deploy_qt_qm
} else:!no_bundle_deploy:mac {
	APP_PREFIX = $${PROJECT_NAME}.app/Contents
	isEmpty(INSTALL_APPS): INSTALL_APPS = $${PREFIX}/.app-tmp
	isEmpty(INSTALL_BINS): INSTALL_BINS = $${PREFIX}/$${APP_PREFIX}/MacOS
	isEmpty(INSTALL_LIBS): INSTALL_LIBS = $${PREFIX}/$${APP_PREFIX}/Frameworks
	isEmpty(INSTALL_HEADERS): INSTALL_HEADERS = $${PREFIX}/$${APP_PREFIX}/Headers
	isEmpty(INSTALL_PLUGINS): INSTALL_PLUGINS = $${PREFIX}/$${APP_PREFIX}/PlugIns
	isEmpty(INSTALL_TRANSLATIONS): INSTALL_TRANSLATIONS = $${PREFIX}/$${APP_PREFIX}/Resources/translations
	isEmpty(INSTALL_SHARE): INSTALL_SHARE = $${PREFIX}/$${APP_PREFIX}/Resources
} else {
	# bins
	!no_bundle_deploy:win32:isEmpty(INSTALL_BINS): INSTALL_BINS = $$PREFIX
	isEmpty(INSTALL_BINS): INSTALL_BINS = $${PREFIX}/bin
	isEmpty(INSTALL_APPS): INSTALL_APPS = $$INSTALL_BINS

	# libs
	!no_bundle_deploy:win32:isEmpty(INSTALL_LIBS): INSTALL_LIBS = $$INSTALL_BINS
	else:isEmpty(INSTALL_LIBS): INSTALL_LIBS = $${PREFIX}/lib

	# headers
	isEmpty(INSTALL_HEADERS): INSTALL_HEADERS = $${PREFIX}/include

	# plugins
	!no_bundle_deploy:win32:isEmpty(INSTALL_PLUGINS): INSTALL_PLUGINS = $$INSTALL_BINS
	else:isEmpty(INSTALL_PLUGINS): INSTALL_PLUGINS = $${PREFIX}/plugins

	# translations
	!no_bundle_deploy:android:isEmpty(INSTALL_TRANSLATIONS):  INSTALL_TRANSLATIONS = /assets/translations
	else:isEmpty(INSTALL_TRANSLATIONS): INSTALL_TRANSLATIONS = $${PREFIX}/translations

	# share
	isEmpty(INSTALL_SHARE): INSTALL_SHARE = $${PREFIX}/share
}

auto_lrelease: PRE_TARGETDEPS += lrelease
android: CONFIG += no_headers_install
