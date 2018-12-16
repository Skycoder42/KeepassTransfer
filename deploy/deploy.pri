DISTFILES += \
	$$PWD/qm-combine.py

# begin deploy script
deploy_target.target = deploy
deploy_target.depends += install
QMAKE_EXTRA_TARGETS += deploy_target

# deploy binaries
!isEmpty(DEPLOY_BINS) {
	# run actual deployment tools
	run_deploy.target = run-deploy-tools
	run_deploy.depends += install
	QMAKE_EXTRA_TARGETS += run_deploy
	deploy_target.depends += run-deploy-tools

	win32 {
		isEmpty(QMAKE_WINDEPLOYQT) {
			qtPrepareTool(QMAKE_WINDEPLOYQT, windeployqt)
			CONFIG(release, debug|release): QMAKE_WINDEPLOYQT += --release
			CONFIG(debug, debug|release): QMAKE_WINDEPLOYQT += --debug
			QMAKE_WINDEPLOYQT += --no-translations $$DEPLOY_EXTRA_ARGS
		}

		for(bin, DEPLOY_BINS): run_deploy.commands += $$QMAKE_WINDEPLOYQT \"$(INSTALL_ROOT)$$bin\"$$escape_expand(\n\t)

		run_deploy.commands += echo [Paths] > \"$(INSTALL_ROOT)$$INSTALL_BINS/qt.conf\" $$escape_expand(\n\t)
		run_deploy.commands += echo Prefix=. >> \"$(INSTALL_ROOT)$$INSTALL_BINS/qt.conf\" $$escape_expand(\n\t)
		run_deploy.commands += echo Binaries=. >> \"$(INSTALL_ROOT)$$INSTALL_BINS/qt.conf\" $$escape_expand(\n\t)
		run_deploy.commands += echo Libraries=. >> \"$(INSTALL_ROOT)$$INSTALL_BINS/qt.conf\" $$escape_expand(\n\t)
		run_deploy.commands += echo Plugins=. >> \"$(INSTALL_ROOT)$$INSTALL_BINS/qt.conf\" $$escape_expand(\n\t)

		run_deploy.commands += $$QMAKE_DEL_FILE "v*redist*exe"
	} else:mac {
		isEmpty(QMAKE_MACDEPLOYQT): qtPrepareTool(QMAKE_MACDEPLOYQT, macdeployqt)
		BINS_COPY = $$DEPLOY_BINS
		BIN_APP = $$take_first(BINS_COPY)
		run_deploy.commands += $$QMAKE_MACDEPLOYQT \"$(INSTALL_ROOT)$$BIN_APP\"
		for(bin, BINS_COPY): run_deploy.commands += \"-executable=$(INSTALL_ROOT)$$bin\"
		run_deploy.commands += $${deploy_target.extra_args}$$escape_expand(\n\t)
		run_deploy.commands += echo $$shell_quote(Translations=Resources/translations) >> \"$(INSTALL_ROOT)$$BIN_APP/Contents/Resources/qt.conf\" $$escape_expand(\n\t)
	} else:android {
		isEmpty(QMAKE_ANDROIDDEPLOYQT): qtPrepareTool(QMAKE_ANDROIDDEPLOYQT, androiddeployqt)
		QMAKE_ANDROIDDEPLOYQT += --deployment bundled --gradle
		CONFIG(release, debug|release) {
			QMAKE_ANDROIDDEPLOYQT += --release --no-gdbserver
			APK_TYPE = release-unsigned
		}
		CONFIG(debug, debug|release) {
			QMAKE_ANDROIDDEPLOYQT += --gdbserver
			APK_TYPE = debug
		}
		for(bin, DEPLOY_BINS) {
			run_deploy.commands += $$QMAKE_ANDROIDDEPLOYQT --input $$shell_path($$bin) --output \"$(INSTALL_ROOT)$$PREFIX\" $$escape_expand(\n\t)
			run_deploy.commands += $$QMAKE_INSTALL_FILE \"$(INSTALL_ROOT)$$PREFIX/build/outputs/apk/`basename \"$(INSTALL_ROOT)$$PREFIX\"`-$${APK_TYPE}.apk\" \"$(INSTALL_ROOT)/syrem-$${VERSION}_$${QT_PLATFORM}_$${APK_TYPE}.apk\"
		}
	}

	# deploy missing plugins (as part of make install)
	for(plgdir, DEPLOY_PLUGINS) {
		plugin_fixup_$${plgdir}.path = $$INSTALL_PLUGINS/$$plgdir
		INSTALLS += plugin_fixup_$${plgdir}

		ALL_PLGS=$$files($$[QT_INSTALL_PLUGINS]/$$plgdir/*)
		PLG_ACTUAL=
		for(plg, ALL_PLGS) {
			PLG_BASE = $$split(plg, ".")
			$$take_last(PLG_BASE)
			PLG_BASE = $$join(PLG_BASE, ".")
			win32: exists($${PLG_BASE}d.dll): PLG_ACTUAL += $${PLG_BASE}
			mac: exists($${PLG_BASE}_debug.dylib): PLG_ACTUAL += $${PLG_BASE}
			linux: exists($${PLG_BASE}.so.debug): PLG_ACTUAL += $${PLG_BASE}
		}

		for(PLG_BASE, PLG_ACTUAL) {
			win32:CONFIG(release, debug|release): PLG_PATH = $${PLG_BASE}.dll
			else:win32:CONFIG(debug, debug|release): PLG_PATH = $${PLG_BASE}d.dll
			else:mac:CONFIG(release, debug|release): PLG_PATH = $${PLG_BASE}.dylib
			else:mac:CONFIG(debug, debug|release): PLG_PATH = $${PLG_BASE}_debug.dylib
			else:linux: PLG_PATH = $${PLG_BASE}.so
			plugin_fixup_$${plgdir}.files += $$PLG_PATH
		}
	}
}

# add package target
package_target.target = package
package_target.commands += cd \"$(INSTALL_ROOT)$$shell_path($$PREFIX)\" &&
win32: package_target.commands += 7z a \"$(INSTALL_ROOT)\\$${PROJECT_TARGET}-$${VERSION}_$${QT_PLATFORM}.zip\" .\\*
else:mac: package_target.commands += hdiutil create -fs HFS+ -srcfolder . -volname $$shell_quote($$PROJECT_NAME) \"$(INSTALL_ROOT)/$${PROJECT_TARGET}-$${VERSION}_$${QT_PLATFORM}.dmg\"
else: package_target.commands += tar cJf \"$(INSTALL_ROOT)/$${PROJECT_TARGET}-$${VERSION}_$${QT_PLATFORM}.tar.xz\" ./*
QMAKE_EXTRA_TARGETS += package_target

# deploy translations
!isEmpty(TS_DICTIONARIES) {
	QMAKE_EXTRA_TARGETS += install

	win32: QMAKE_QM_COMBINE = python
	QMAKE_QM_COMBINE += $$shell_path($$PWD/qm-combine.py)
	!no_deploy_qt_qm: QMAKE_QM_COMBINE += --deploy-qt
	QMAKE_QM_COMBINE += $$shell_path($$[QT_INSTALL_TRANSLATIONS])
	isEmpty(QMAKE_LRELEASE): qtPrepareTool(QMAKE_LRELEASE, lrelease)
	for(tsdict, TS_DICTIONARIES): install.commands += $$QMAKE_QM_COMBINE $$shell_quote($$tsdict) \"$(INSTALL_ROOT)$$INSTALL_TRANSLATIONS\" $$QMAKE_LRELEASE $$escape_expand(\n\t)
}
