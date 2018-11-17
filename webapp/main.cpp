#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QZXing.h>

#include <serverconnector.h>

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);
	QZXing::registerQMLTypes();

	ServerConnector connector{QStringLiteral("ws://localhost:27352")};

	QQmlApplicationEngine engine;
	QZXing::registerQMLImageProvider(engine);
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
