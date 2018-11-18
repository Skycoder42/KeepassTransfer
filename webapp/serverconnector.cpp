#include "serverconnector.h"
#include <messages/appidentmessage.h>
#include <QTimer>

ServerConnector::ServerConnector(QUrl url, QObject *parent) :
	QObject{parent},
	_serverUrl{std::move(url)}
{
	recreateSocket();
}

bool ServerConnector::isConnected() const
{
	return _socket;
}

QUuid ServerConnector::appId() const
{
	return _appId;
}

void ServerConnector::connected()
{
	_retryTimeout = 1000;
	qDebug() << "Connection to server established. Sending AppIdent";
	_socket->sendBinaryMessage(KPTLib::serializeMessage(AppIdentMessage{}));

	emit connectedChanged(isConnected(), {});
}

void ServerConnector::disconnected()
{
	qWarning() << "Connection to server lost. Retrying in" << _retryTimeout << "ms ...";
	_socket->disconnect(this);
	_socket->deleteLater();
	_socket = nullptr;
	_appId = {};
	QTimer::singleShot(_retryTimeout, this, &ServerConnector::recreateSocket);
	_retryTimeout = std::min(_retryTimeout * 2, 30000);

	emit connectedChanged(isConnected(), {});
	emit appIdChanged(_appId, {});
}

void ServerConnector::error(QAbstractSocket::SocketError error)
{
	if(error == QAbstractSocket::RemoteHostClosedError)
		return;

	qWarning() << "Socket-Error occured:" << _socket->errorString();
}

void ServerConnector::binaryMessageReceived(const QByteArray &message)
{
	KPTLib::MessageVisitor visitor;
	visitor.addFallbackVisitor(this, &ServerConnector::onFallback);
	visitor.addVisitor(this, &ServerConnector::onServerIdent);
	visitor.addVisitor(this, &ServerConnector::onError);
	visitor.visit(message);
}

void ServerConnector::recreateSocket()
{
	// cleanup socket if still connected
	if(_socket) {
		_socket->disconnect(this);
		connect(_socket, &QWebSocket::disconnected,
				_socket, &QWebSocket::deleteLater);
		_socket->close();
		_socket = nullptr;
		emit connectedChanged(isConnected(), {});
	}

	// cleanup appid if still set
	if(!_appId.isNull()) {
		_appId = {};
		emit appIdChanged(_appId, {});
	}

	// create and connect the new socket
	_socket = new QWebSocket{_serverUrl.authority(), QWebSocketProtocol::VersionLatest, this};
	connect(_socket, &QWebSocket::connected,
			this, &ServerConnector::connected);
	connect(_socket, &QWebSocket::disconnected,
			this, &ServerConnector::disconnected);
	connect(_socket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
			this, &ServerConnector::error);
	connect(_socket, &QWebSocket::binaryMessageReceived,
			this, &ServerConnector::binaryMessageReceived);

	_socket->open(_serverUrl);
}

void ServerConnector::onServerIdent(const ServerIdentMessage message)
{
	_appId = message.channelId;
	qDebug() << message.channelId;
	emit appIdChanged(_appId, {});
}

void ServerConnector::onError(const ErrorMessage &message)
{
	emit serverError(message.message);
	_socket->close();
}

void ServerConnector::onFallback(int typeId)
{
	qWarning() << "Invalid message received from server - unexpected message type:"
			   << QMetaType::typeName(typeId);
	_socket->close(QWebSocketProtocol::CloseCodeBadOperation);
}
