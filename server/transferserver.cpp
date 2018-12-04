#include "transferserver.h"
#include <qtcoroutine.h>
#include <qtcoawaitables.h>
#include <kptlib.h>

#include <messages/serveridentmessage.h>
#include <messages/servertransfermessage.h>
#include <messages/serverokmessage.h>

#include <QTimer>

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
				visitor.addVisitor(this, &TransferServer::onClientTransfer, socket);

				// wait for the first message
				visitor.visit(QtCoroutine::awaitargs<QByteArray>::await(socket, &QWebSocket::binaryMessageReceived));
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

void TransferServer::onInvalidMessage2(int typeId, QWebSocket *socket, QWebSocket *partnerSocket)
{
	onInvalidMessage(typeId, socket);
	partnerSocket->sendBinaryMessage(KPTLib::serializeMessage(ErrorMessage{tr("Invalid data received from WebApp.")}));
	closeOnTimeout(partnerSocket);
}

void TransferServer::onAppIdent(const AppIdentMessage message, QWebSocket *socket)
{
	// verify version
	if(!verifyProtocolVersion(message.version, socket))
		return;

	// generate id and register the app
	QUuid id;
	do {
		id = QUuid::createUuid();
	} while(_appHash.contains(id));

	_appHash.insert(id, socket);
	connect(socket, &QWebSocket::disconnected,
			this, [this, id](){
		_appHash.remove(id);
	});

	qDebug() << "WebApp socket " << socket->peerAddress() << "connected for id" << id;
	socket->sendBinaryMessage(KPTLib::serializeMessage(ServerIdentMessage{id}));
}

void TransferServer::onClientTransfer(ClientTransferMessage message, QWebSocket *clientSocket)
{
	// verify version
	if(!verifyProtocolVersion(message.version, clientSocket))
		return;

	// verify webapp exists
	auto appSocket = _appHash.value(message.channelId);
	if(!appSocket) {
		qDebug() << "Client socket " << clientSocket->peerAddress() << "specified an unknown WebApp ID:" << message.channelId;
		clientSocket->sendBinaryMessage(KPTLib::serializeMessage(ErrorMessage{
			tr("Unknown Channel-ID. The WebApp you want to connect to does not exist!")
		}));
		closeOnTimeout(clientSocket);
		return;
	}

	// prepare visitor for message reply
	KPTLib::MessageVisitor visitor;
	visitor.addFallbackVisitor(this, &TransferServer::onInvalidMessage2, appSocket, clientSocket);
	visitor.addVisitor(this, &TransferServer::onAppOk, clientSocket);
	visitor.addVisitor(this, &TransferServer::onAppError, clientSocket);

	// forward message data and wait for the reply
	appSocket->sendBinaryMessage(KPTLib::serializeMessage(ServerTransferMessage{std::move(message.data)}));
	visitor.visit(QtCoroutine::awaitargs<QByteArray>::await(appSocket, &QWebSocket::binaryMessageReceived));
}

void TransferServer::onAppOk(const AppOkMessage &message, QWebSocket *clientSocket)
{
	Q_UNUSED(message);
	clientSocket->sendBinaryMessage(KPTLib::serializeMessage(ServerOkMessage{}));
	closeOnTimeout(clientSocket, 10000); //allow longer timeout for "successful" operations
}

void TransferServer::onAppError(const ErrorMessage &message, QWebSocket *clientSocket)
{
	clientSocket->sendBinaryMessage(KPTLib::serializeMessage(message));
	closeOnTimeout(clientSocket);
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

bool TransferServer::verifyProtocolVersion(quint32 version, QWebSocket *socket)
{
	if(version != KPTLib::ProtocolVersion) {
		socket->sendBinaryMessage(KPTLib::serializeMessage(ErrorMessage{
			tr("Server uses different message protocol. "
			   "It uses version %1, whilest you are using version %2. "
			   "Please update your application to get support for the newer version.")
			.arg(KPTLib::ProtocolVersion)
			.arg(version)
		}));
		closeOnTimeout(socket);
		return false;
	} else
		return true;
}

void TransferServer::closeOnTimeout(QWebSocket *socket, int timeout)
{
	QTimer::singleShot(timeout, socket, std::bind(&QWebSocket::close, socket, QWebSocketProtocol::CloseCodeBadOperation, QString{}));
}
