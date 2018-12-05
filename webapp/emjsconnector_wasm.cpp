#include "emjsconnector.h"
#include <QGuiApplication>
#include <QGlobalStatic>
#include <QDebug>

#include <emscripten/bind.h>
#include <emscripten/val.h>
using namespace emscripten;

Q_GLOBAL_STATIC(EmJsConnector, clipInstance)

namespace {

void onPasteEvent(val event) {
	event.call<void>("preventDefault");
	//auto text = event["clipboardData"].call<val>("getData", std::string{"text/plain"});
	//qDebug() << "event CP data:" << text.as<std::string>().c_str();
	clipInstance->readText();
}

void onGetTextCallback(val text) {
	clipInstance->updateClipboard(QString::fromStdString(text.as<std::string>()));
}

void onPopState(val event) {
	Q_UNUSED(event)
	auto location = val::global("location");
	emit clipInstance->tagChanged(QString::fromStdString(location["hash"].as<std::string>()).mid(1));
}

}

EMSCRIPTEN_BINDINGS(EmJsConnectorModule) {
	function("onPasteEvent", &onPasteEvent);
	function("onGetTextCallback", &onGetTextCallback);
	function("onPopState", &onPopState);
}

EmJsConnector::EmJsConnector(QObject *parent) :
	QObject{parent},
	_qtClipboard{QGuiApplication::clipboard()}
{
	connect(_qtClipboard, &QClipboard::dataChanged,
			this, &EmJsConnector::qtDataChanged);
	QMetaObject::invokeMethod(this, "installHandlers", Qt::QueuedConnection);
}

EmJsConnector *EmJsConnector::instance()
{
	return clipInstance;
}

void EmJsConnector::readText()
{
	auto navigator = val::global("navigator");
	auto clipboard = navigator["clipboard"];
	auto promise = clipboard.call<val>("readText");
	promise.call<val>("then", val::module_property("onGetTextCallback"));
}

void EmJsConnector::updateClipboard(const QString &text)
{
	_skipNext = true;
	_qtClipboard->setText(text);
}

QUrl EmJsConnector::getHostUrl() const
{
#ifdef QT_NO_DEBUG
	auto location = val::global("location");
	return QStringLiteral("wss://%1/backend").arg(QString::fromStdString(location["host"].as<std::string>()));
#else
	return QStringLiteral("ws://localhost:27352");
#endif
}

void EmJsConnector::setTag(const QString &tag)
{
	auto location = val::global("location");
	location.set("hash", "#" + tag.toStdString());
}

void EmJsConnector::openUrl(const QUrl &url)
{
	auto window = val::global("window");
	window.call<val>("open",
					 url.toString(QUrl::FullyEncoded).toStdString(),
					 std::string{"_blank"});
}

void EmJsConnector::copyText(const QString &text)
{
	_qtClipboard->setText(text);
}

void EmJsConnector::qtDataChanged()
{
	if(_skipNext) {
		_skipNext = false;
		return;
	}

	auto navigator = val::global("navigator");
	auto clipboard = navigator["clipboard"];
	clipboard.call<val>("writeText", _qtClipboard->text().toStdString());
}

void EmJsConnector::installHandlers()
{
	auto document = val::global("document");
	document.call<void>("addEventListener",
						std::string{"paste"},
						val::module_property("onPasteEvent"));
	auto window = val::global("window");
	window.set("onpopstate", val::module_property("onPopState"));
}
