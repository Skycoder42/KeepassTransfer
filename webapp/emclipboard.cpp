#include "emclipboard.h"
#include <QGuiApplication>
#include <QDebug>

#include <emscripten/bind.h>
#include <emscripten/val.h>
using namespace emscripten;

namespace {

void test1(val self) {
	qDebug() << Q_FUNC_INFO << self.typeOf().as<std::string>().c_str();
}

void test2(val self) {
	qDebug() << Q_FUNC_INFO << self.typeOf().as<std::string>().c_str();
}

void test3(val self) {
	qDebug() << Q_FUNC_INFO << self.typeOf().as<std::string>().c_str();
}

}

EMSCRIPTEN_BINDINGS(EmClipboardModule) {
	function("test1", &test1);
	function("test2", &test2);
	function("test3", &test3);
}

EmClipboard::EmClipboard(QObject *parent) :
	QObject{parent},
	_qtClipboard{QGuiApplication::clipboard()}
{
	connect(_qtClipboard, &QClipboard::dataChanged,
			this, &EmClipboard::qtDataChanged);
	qtDataChanged();
}

void EmClipboard::qtDataChanged()
{
	auto navigator = val::global("navigator");
	auto clipboard = navigator["clipboard"];
	auto promise = clipboard.call<val>("writeText", _qtClipboard->text().toStdString());
	promise.call<val>("then", val::module_property("test1"))
			.call<val>("catch", val::module_property("test2"))
			.call<val>("finally", val::module_property("test3"));
}
