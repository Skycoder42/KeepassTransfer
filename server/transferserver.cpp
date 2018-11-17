#include "transferserver.h"
#include <qtcoroutine.h>
#include <qtcoawaitables.h>
#include <kptlib.h>

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
	while(_server->hasPendingConnections()) {
		QtCoroutine::createAndRun([this](){
			do {
				auto socket = _server->nextPendingConnection();
				setupCleanupConnections(socket);

				KPTLib::MessageVisitor visitor;
				visitor.addFallbackVisitor(this, &TransferServer::onInvalidMessage, socket);
				visitor.addVisitor(this, &TransferServer::onAppIdent, socket);

				// wait for the first message
				//TODO improve in coroutine
				const auto message = QtCoroutine::awaitargs<QByteArray>::await(socket, &QWebSocket::binaryMessageReceived);
				visitor.visit(message);
			} while(_server->hasPendingConnections());
		});
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

void TransferServer::onInvalidMessage(int typeId, QWebSocket *socket)
{
	qWarning().noquote() << "Invalid message received by" << socket->peerAddress()
						 << "- unexpected message type:" << QMetaType::typeName(typeId);
	socket->close(QWebSocketProtocol::CloseCodeBadOperation);
}

void TransferServer::onAppIdent(const AppIdentMessage &message, QWebSocket *socket)
{
	qDebug() << "AppIdentMessage received by" << socket->peerAddress()
			 << "with" << message.version;
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

void TransferServer::setupCleanupConnections(QWebSocket *socket)
{
	connect(socket, &QWebSocket::destroyed,
			this, std::bind(&QtCoroutine::cancel, QtCoroutine::current()),
			Qt::QueuedConnection);
	connect(socket, &QWebSocket::disconnected,
			socket, [socket](){
		qDebug() << "Client with address" << socket->peerAddress() << "has disconnected";
		socket->deleteLater();
	});
	connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
			socket, [socket](QAbstractSocket::SocketError error) {
		if(error == QAbstractSocket::RemoteHostClosedError)
			return;
		qWarning().noquote() << "Error on peer with address" << socket->peerAddress() << "occured:" << socket->errorString();
		if(socket->state() == QAbstractSocket::ConnectedState)
			socket->close();
	}, Qt::QueuedConnection);
}
