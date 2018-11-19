#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QZXing.h>

#include <serverconnector.h>
#include <dataencryptor.h>

#include "qrencoder.h"
#ifdef QT_OS_WASM
#include "browserstorage.h"
#endif

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);

	QZXing::registerQMLTypes();
	qmlRegisterUncreatableType<ServerConnector>("de.skycoder42.kpt", 1, 0, "ServerConnector", {});
	qmlRegisterUncreatableType<DataEncryptor>("de.skycoder42.kpt", 1, 0, "DataEncryptor", {});
	qmlRegisterType<QrEncoder>("de.skycoder42.kpt", 1, 0, "QrEncoder");

#ifdef QT_OS_WASM
	BrowserStorage storage;
	qDebug() << storage.keyCount();
	for(auto i = 0, total = storage.keyCount(); i < total; ++i) {
		qDebug() << storage.key(i);
		qDebug() << storage.value(storage.key(i));
	}

	storage.clear();
	storage.setValue(QStringLiteral("test1"), 42);
	storage.setValue(QStringLiteral("test2"), 24);
	qDebug() << storage.value(QStringLiteral("test1"))
			 << storage.value(QStringLiteral("test2"));
	storage.remove(QStringLiteral("test2"));
	qDebug() << storage.value(QStringLiteral("test2"));
#endif

	ServerConnector connector{QStringLiteral("ws://localhost:27352")};

	QQmlApplicationEngine engine;
	QZXing::registerQMLImageProvider(engine);
	engine.rootContext()->setContextProperty(QStringLiteral("connector"), &connector);

	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
