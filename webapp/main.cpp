#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QWebSocket>
#include <QZXing.h>

void testCryptopp();

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

	auto socket = new QWebSocket{};
	QObject::connect(socket, &QWebSocket::connected, [=](){
		qDebug() << "socket connected";
		socket->sendBinaryMessage("Hello World");
	});
	QObject::connect(socket, &QWebSocket::binaryMessageReceived, [=](const QByteArray &msg){
		qDebug() << "socket received:" << msg;
		socket->sendBinaryMessage("Hello World");
	});
	QObject::connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
			[=](){
		qWarning() << socket->errorString();
	});
	socket->open(QStringLiteral("ws://localhost:27352"));

	testCryptopp();

	return app.exec();
}
