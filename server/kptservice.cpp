#include "kptservice.h"

#include <QThread>

KPTService::KPTService(int &argc, char **argv) :
	Service{argc, argv}
{
	QCoreApplication::setApplicationName(QStringLiteral(PROJECT_TARGET));
	QCoreApplication::setApplicationVersion(QStringLiteral(VERSION));
	QCoreApplication::setOrganizationName(QStringLiteral(COMPANY));
	QCoreApplication::setOrganizationDomain(QStringLiteral(BUNDLE));
}

QtService::Service::CommandResult KPTService::onStart()
{
	_server = new QWebSocketServer{
		QStringLiteral("localhost"), //TODO set dynamically
		QWebSocketServer::NonSecureMode,
		this
	};

	//DEBUG test
	connect(_server, &QWebSocketServer::newConnection,
			this, [=](){
		while(_server->hasPendingConnections()) {
			auto socket = _server->nextPendingConnection();
			connect(socket, &QWebSocket::binaryMessageReceived,
					this, [=](const QByteArray &msg){
				qDebug() << msg;
				QThread::sleep(1);
				socket->sendBinaryMessage("Hello World");
			});
			connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
					this, [=](){
				qWarning() << socket->errorString();
			});
		}
	});

	auto socket = getSocket();
	auto ok = false;
	if(socket != -1)
		ok = _server->setSocketDescriptor(socket);
	else {
		ok = _server->listen(QHostAddress::LocalHost,
							 27352); //TODO make configurable
	}

	if(!ok) {
		qCritical().noquote() << _server->errorString();
		return OperationFailed;
	} else
		qInfo() << "Listening on port" << _server->serverPort();

	return OperationCompleted;
}

QtService::Service::CommandResult KPTService::onStop(int &exitCode)
{
	exitCode = EXIT_SUCCESS;
	return OperationCompleted;
}
