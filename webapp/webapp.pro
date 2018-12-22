TEMPLATE = app

QT += quick websockets svg quickcontrols2
CONFIG(release, debug|release): CONFIG += qtquickcompiler

QTPLUGIN.imageformats = qsvg

# hack to exclude QML plugins:
ADDED_IMPORTS += \
	qtquickcontrols2universalstyleplugin \
	qtquickcontrols2fusionstyleplugin \
	qtquickcontrols2imaginestyleplugin

TARGET = $${PROJECT_TARGET}-webapp

QMAKE_TARGET_DESCRIPTION = "$$PROJECT_NAME Webapp"
RC_ICONS += ../../icon/kpt.ico
ICON += ../../icon/kpt.icns

HEADERS += \
	serverconnector.h \
	iencoder.h \
	qrencoder.h \
	qrimageprovider.h \
	emjsconnector.h \
	passencoder.h

SOURCES += \
	main.cpp \
	serverconnector.cpp \
	iencoder.cpp \
	qrencoder.cpp \
	qrimageprovider.cpp \
	passencoder.cpp

RESOURCES += \
	webapp.qrc

wasm {
	HEADERS += \
		qwasmsettings.h

	SOURCES += \
		qwasmsettings.cpp \
		emjsconnector_wasm.cpp
} else {
	SOURCES += \
		emjsconnector.cpp
}

include($$SRC_ROOT_DIR/lib/lib.pri)
include($$SRC_ROOT_DIR/3rdparty/QR-Code-generator/QR-Code-generator.pri)
include($$SRC_ROOT_DIR/deploy/install.pri)

wasm {
	# adjust HTML file
	html_fixup_target.target = index.html
	html_fixup_target.depends = shellfiles

	REP_STR =
	WASM_ICON_SIZES = 16 32 48 64 96 128 256 512
	for(pngSize, WASM_ICON_SIZES) {
		equals(pngSize, "16") {
			REL_STR="shortcut icon"
			SIZE_STR=
		} else {
			REL_STR="icon"
			SIZE_STR="sizes=\"$${pngSize}x$${pngSize}\""
		}
		REP_STR += "<link rel=\"$${REL_STR}\" type=\"image/png\" href=\"kpt_$${pngSize}.png\" $$SIZE_STR>"
		wasm_install.files += "../icon/pngs/kpt_$${pngSize}.png"
	}
	html_fixup_target.commands += sed $$shell_quote(s:<title>$$TARGET</title>:<title>$$PROJECT_NAME</title>$$REP_STR:g) $$shell_path($$OUT_PWD/$${TARGET}.html) > index.html
	QMAKE_EXTRA_TARGETS += html_fixup_target
	PRE_TARGETDEPS += index.html

	# shellfile target hack
	shellfiles_target.target = shellfiles
	shellfiles_target.commands += \
		@touch apphtml \
		$$escape_expand(\n\t)@touch appjs \
		$$escape_expand(\n\t)@touch appsvg \
		$$escape_expand(\n\t)@touch shellfiles
	QMAKE_EXTRA_TARGETS += shellfiles_target

	wasm_install.files += $$OUT_PWD/index.html \
		$$OUT_PWD/$${TARGET}.wasm \
#		$$OUT_PWD/$${TARGET}.js \
		$$OUT_PWD/qtloader.js \
		$$OUT_PWD/qtlogo.svg
	wasm_install.CONFIG += no_check_exist
	wasm_install.path = $$INSTALL_WWW
	target.path = $$INSTALL_WWW
	INSTALLS += wasm_install
} else {
	target.path = $$INSTALL_BINS
	INSTALLS += target
}

QMAKE_EXTRA_TARGETS += lrelease
