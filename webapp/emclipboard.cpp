#include "emclipboard.h"
#include <QGuiApplication>
#include <QDebug>

#include <emscripten/bind.h>
#include <emscripten/val.h>
using namespace emscripten;

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
	clipboard.call<val>("writeText", _qtClipboard->text().toStdString());
}
