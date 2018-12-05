#include "serverconnector.h"
#include <QTimer>
#include <messages/appidentmessage.h>
#include <messages/appokmessage.h>
#include "iencoder.h"

ServerConnector::ServerConnector(QUrl url, QObject *parent) :
	QObject{parent},
	_serverUrl{std::move(url)}
{
	qInfo() << "Connecting to remote backend server with url:" << _serverUrl;
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

QString ServerConnector::appIdStr() const
{
	return _appId.toString(QUuid::WithoutBraces).toUpper();
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
	emit appIdChanged({});
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
	visitor.addVisitor(this, &ServerConnector::onServerTransfer);
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
		emit appIdChanged({});
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
	emit appIdChanged({});
}

void ServerConnector::onError(const ErrorMessage &message)
{
	emit serverError(message.message);
	_socket->close();
}

void ServerConnector::onServerTransfer(const ServerTransferMessage &message)
{
	const auto credentials = IEncoder::decodeData(message.data);
	if(credentials.isEmpty()){
		_socket->sendBinaryMessage(KPTLib::serializeMessage(ErrorMessage{
			tr("Invalid message data. WebApp was unable to decode the received data")
		}));
	} else {
		_socket->sendBinaryMessage(KPTLib::serializeMessage(AppOkMessage{}));
		QVariantList credList;
		credList.reserve(credentials.size());
		auto entryTitle = tr("<Unnamed>");
		for(const auto &cred : credentials) {
			if(cred.key() == QStringLiteral("Title") && !cred.confidential())
				entryTitle = cred.value();
			else
				credList.append(QVariant::fromValue(cred));
		}
		emit credentialsReceived(credList, entryTitle);
	}
}

void ServerConnector::onFallback(int typeId)
{
	qWarning() << "Invalid message received from server - unexpected message type:"
			   << QMetaType::typeName(typeId);
	_socket->close(QWebSocketProtocol::CloseCodeBadOperation);
}
