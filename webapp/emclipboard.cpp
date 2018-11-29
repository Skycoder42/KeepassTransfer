#include "emclipboard.h"
#include <QGuiApplication>
#include <QGlobalStatic>
#include <QDebug>

#include <emscripten/bind.h>
#include <emscripten/val.h>
using namespace emscripten;

Q_GLOBAL_STATIC(EmClipboard, clipInstance)

namespace {

void onPasteEvent(val event) {
	event.call<void>("preventDefault");
	clipInstance->readText();
}

void onGetTextCallback(val text) {
	clipInstance->updateClipboard(QString::fromStdString(text.as<std::string>()));
}

}

EMSCRIPTEN_BINDINGS(EmClipboardModule) {
	function("onPasteEvent", &onPasteEvent);
	function("onGetTextCallback", &onGetTextCallback);
}

EmClipboard::EmClipboard(QObject *parent) :
	QObject{parent},
	_qtClipboard{QGuiApplication::clipboard()}
{
	connect(_qtClipboard, &QClipboard::dataChanged,
			this, &EmClipboard::qtDataChanged);
	QMetaObject::invokeMethod(this, "installPasteHandler", Qt::QueuedConnection);
}

EmClipboard *EmClipboard::instance()
{
	return clipInstance;
}

void EmClipboard::readText()
{
	auto navigator = val::global("navigator");
	auto clipboard = navigator["clipboard"];
	auto promise = clipboard.call<val>("readText");
	promise.call<val>("then", val::module_property("onGetTextCallback"));
}

void EmClipboard::updateClipboard(const QString &text)
{
	_skipNext = true;
	_qtClipboard->setText(text);
}

void EmClipboard::qtDataChanged()
{
	if(_skipNext) {
		_skipNext = false;
		return;
	}

	auto navigator = val::global("navigator");
	auto clipboard = navigator["clipboard"];
	clipboard.call<val>("writeText", _qtClipboard->text().toStdString());
}

void EmClipboard::installPasteHandler()
{
	auto document = val::global("document");
	document.call<void>("addEventListener",
						std::string{"paste"},
						val::module_property("onPasteEvent"));
}
