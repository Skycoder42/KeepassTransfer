#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QZXing.h>

#include <serverconnector.h>

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);
	QZXing::registerQMLTypes();
	qmlRegisterUncreatableType<ServerConnector>("de.skycoder42.kpt", 1, 0, "ServerConnector", {});

	ServerConnector connector{QStringLiteral("ws://localhost:27352")};

	QQmlApplicationEngine engine;
	QZXing::registerQMLImageProvider(engine);
	engine.rootContext()->setContextProperty(QStringLiteral("connector"), &connector);

	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
