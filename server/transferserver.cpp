#include "transferserver.h"
#include <qtcoroutine.h>
#include <qtcoawaitables.h>
#include <kptlib.h>

#include <messages/serveridentmessage.h>
#include <messages/servertransfermessage.h>
#include <messages/serverokmessage.h>

#include <QTimer>

TransferServer::TransferServer(qint64 timeout, QObject *parent) :
	QObject{parent},
	_timeoutTimer{new QTimer{this}},
	_timeout{timeout}
{
	using namespace std::chrono_literals;
	_timeoutTimer->setTimerType(Qt::VeryCoarseTimer);
	connect(_timeoutTimer, &QTimer::timeout,
			this, &TransferServer::timeout);
	_timeoutTimer->start(5s);
}

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

#ifndef QT_NO_DEBUG
	qDebug() << "New WebApp socket" << socket->peerAddress() << "connected for id" << id;
#endif
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
		qDebug() << "Client socket" << clientSocket->peerAddress() << "specified an unknown WebApp ID:" << message.channelId;
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

void TransferServer::timeout()
{
	for(auto it = _timeouts.begin(); it != _timeouts.end(); ++it) {
		if(it->first.hasExpired()) {
			const auto socket = it.key();
			if(it->second) {
				qCritical().noquote() << "Disconnecting" << socket->peerAddress()
									  << "after timeout of ping message";
				socket->close(QWebSocketProtocol::CloseCodeBadOperation);
			} else {
				it->second = true;
				const auto killId = QUuid::createUuid();
				QtCoroutine::createAndRun([this, socket, killId](){
					connect(socket, &QWebSocket::destroyed,
							this, std::bind(&QtCoroutine::cancel, QtCoroutine::current()),
							Qt::QueuedConnection);

					socket->ping(killId.toRfc4122());
					auto [delay, payload] = QtCoroutine::await(socket, &QWebSocket::pong);
					qDebug().noquote() << "Received pong of" << socket->peerAddress()
									   << "with delay:" << delay;
					if(payload == killId.toRfc4122())
						_timeouts[socket] = {QDeadlineTimer{_timeout}, false};
					else {
						qCritical().noquote() << "Disconnecting" << socket->peerAddress()
											  << "after receiving invalid pong response";
						socket->close(QWebSocketProtocol::CloseCodeBadOperation);
					}
				});
			}
		}
	}
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
			socket, [this, socket](){
		qDebug() << "Client with address" << socket->peerAddress() << "has disconnected";
		_timeouts.remove(socket);
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
	_timeouts.insert(socket, {QDeadlineTimer{_timeout}, false});
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
