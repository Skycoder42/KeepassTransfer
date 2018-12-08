#include "emjsconnector.h"
#include <QGlobalStatic>
#include <QDesktopServices>
#include <QGuiApplication>
#include <QClipboard>

Q_GLOBAL_STATIC(EmJsConnector, clipInstance)

EmJsConnector::EmJsConnector(QObject *parent) :
	QObject{parent}
{}

EmJsConnector *EmJsConnector::instance()
{
	return clipInstance;
}

void EmJsConnector::updateClipboard(const QString &) {}

QUrl EmJsConnector::getHostUrl() const
{
#ifdef QT_NO_DEBUG
	return QStringLiteral("wss://kpt.skycoder42.de/backend");
#else
	return QStringLiteral("ws://localhost:27352");
#endif
}

void EmJsConnector::setTag(const QString &) {}

void EmJsConnector::openUrl(const QUrl &url)
{
	QDesktopServices::openUrl(url);
}

void EmJsConnector::copyText(const QString &text)
{
	QGuiApplication::clipboard()->setText(text);
}

void EmJsConnector::qtDataChanged() {}

void EmJsConnector::readJsClipboard() {}
