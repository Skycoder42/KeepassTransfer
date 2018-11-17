#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QWebSocket>
#include <QZXing.h>

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);
	QZXing::registerQMLTypes();

	QQmlApplicationEngine engine;
	QZXing::registerQMLImageProvider(engine);
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
