#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QZXing.h>

#include <serverconnector.h>
#include <dataencryptor.h>

#include "qrencoder.h"
#include "browserstorage.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);

	QZXing::registerQMLTypes();
	qmlRegisterUncreatableType<ServerConnector>("de.skycoder42.kpt", 1, 0, "ServerConnector", {});
	qmlRegisterUncreatableType<DataEncryptor>("de.skycoder42.kpt", 1, 0, "DataEncryptor", {});
	qmlRegisterType<QrEncoder>("de.skycoder42.kpt", 1, 0, "QrEncoder");
	qmlRegisterType<BrowserStorage>("de.skycoder42.kpt", 1, 0, "BrowserStorage");

	ServerConnector connector{QStringLiteral("ws://localhost:27352")};

	QQmlApplicationEngine engine;
	QZXing::registerQMLImageProvider(engine);
	engine.rootContext()->setContextProperty(QStringLiteral("connector"), &connector);

	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
