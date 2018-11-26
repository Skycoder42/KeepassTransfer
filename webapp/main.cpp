#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <serverconnector.h>
#include <dataencryptor.h>

#include "qrencoder.h"
#include "browserstorage.h"
#include "qrimageprovider.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);
	QCoreApplication::setApplicationName(QStringLiteral(PROJECT_TARGET));
	QCoreApplication::setApplicationVersion(QStringLiteral(VERSION));
	QCoreApplication::setOrganizationName(QStringLiteral(COMPANY));
	QCoreApplication::setOrganizationDomain(QStringLiteral(BUNDLE));
	QGuiApplication::setApplicationDisplayName(QStringLiteral(PROJECT_NAME));
	QGuiApplication::setWindowIcon(QIcon{QStringLiteral(":/icons/locked.svg")}); //TODO dummy

	KPTLib::setup();

	qmlRegisterUncreatableType<ServerConnector>("de.skycoder42.kpt", 1, 0, "ServerConnector", {});
	qmlRegisterUncreatableType<DataEncryptor>("de.skycoder42.kpt", 1, 0, "DataEncryptor", {});
	qmlRegisterType<QrEncoder>("de.skycoder42.kpt", 1, 0, "QrEncoder");
	qmlRegisterType<BrowserStorage>("de.skycoder42.kpt", 1, 0, "BrowserStorage");

	ServerConnector connector{QStringLiteral("ws://localhost:27352")};

	QQmlApplicationEngine engine;
	engine.addImageProvider(QStringLiteral("qrcode"), new QrImageProvider{});
	engine.rootContext()->setContextProperty(QStringLiteral("connector"), &connector);

	engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
