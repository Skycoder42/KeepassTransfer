#include "transferserver.h"

TransferServer::TransferServer(QObject *parent) :
	QObject{parent}
{}

bool TransferServer::startServer(const QString &serverName, int socket)
{
	_server = new QWebSocketServer{serverName, QWebSocketServer::NonSecureMode, this};
	if(_server->setSocketDescriptor(socket)) {
		setup();
		qInfo() << "Server activated on existing socket for port" << _server->serverPort();
		return true;
	} else {
		qCritical().noquote() << _server->errorString();
		return false;
	}
}

bool TransferServer::startServer(const QString &serverName, bool localHostOnly, quint16 port)
{
	_server = new QWebSocketServer{serverName, QWebSocketServer::NonSecureMode, this};
	if(_server->listen(localHostOnly ? QHostAddress::LocalHost : QHostAddress::Any, port)) {
		setup();
		qInfo() << "Server listening on port" << _server->serverPort();
		return true;
	} else {
		qCritical().noquote() << _server->errorString();
		return false;
	}
}

void TransferServer::newConnection()
{
	while (_server->hasPendingConnections()) {
		auto socket = _server->nextPendingConnection();
		//TODO here
	}
}

void TransferServer::acceptError(QAbstractSocket::SocketError socketError)
{
	qWarning().noquote() << "Accept-Error [" << socketError << "]:" << _server->errorString();
}

void TransferServer::serverError(QWebSocketProtocol::CloseCode closeCode)
{
	qWarning().noquote() << "Server-Error [" << closeCode << "]:" << _server->errorString();
}

void TransferServer::setup()
{
	connect(_server, &QWebSocketServer::newConnection,
			this, &TransferServer::newConnection);
	connect(_server, &QWebSocketServer::acceptError,
			this, &TransferServer::acceptError);
	connect(_server, &QWebSocketServer::serverError,
			this, &TransferServer::serverError);
}
