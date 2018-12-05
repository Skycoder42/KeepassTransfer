#include "emjsconnector.h"
#include <QGlobalStatic>
#include <QDesktopServices>

Q_GLOBAL_STATIC(EmJsConnector, clipInstance)

EmJsConnector::EmJsConnector(QObject *parent) :
	QObject{parent}
{}

EmJsConnector *EmJsConnector::instance()
{
	return clipInstance;
}

void EmJsConnector::readText() {}

void EmJsConnector::updateClipboard(const QString &) {}

void EmJsConnector::setTag(const QString &) {}

void EmJsConnector::openUrl(const QUrl &url)
{
	QDesktopServices::openUrl(url);
}

void EmJsConnector::qtDataChanged() {}

void EmJsConnector::installHandlers() {}
