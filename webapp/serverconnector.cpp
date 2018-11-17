#include "serverconnector.h"
#include <messages/appidentmessage.h>
#include <QTimer>

ServerConnector::ServerConnector(QUrl url, QObject *parent) :
	QObject{parent},
	_serverUrl{std::move(url)}
{
	recreateSocket();
}

void ServerConnector::connected()
{
	_retryTimeout = 1000;
	qDebug() << "Connection to server established. Sending AppIdent";
	_socket->sendBinaryMessage(KPTLib::serializeMessage(AppIdentMessage{}));
}

void ServerConnector::disconnected()
{
	qWarning() << "Connection to server lost. Retrying in" << _retryTimeout << "ms ...";
	_socket->disconnect(this);
	_socket->deleteLater();
	_socket = nullptr;
	QTimer::singleShot(_retryTimeout, this, &ServerConnector::recreateSocket);
	_retryTimeout = std::min(_retryTimeout * 2, 30000);
}

void ServerConnector::error(QAbstractSocket::SocketError error)
{
	if(error == QAbstractSocket::RemoteHostClosedError)
		return;

	qWarning() << "Socket-Error occured:" << _socket->errorString();
}

void ServerConnector::binaryMessageReceived(const QByteArray &message)
{
	qDebug() << message;
}

void ServerConnector::recreateSocket()
{
	qDebug(Q_FUNC_INFO);

	if(_socket) {
		_socket->disconnect(this);
		connect(_socket, &QWebSocket::disconnected,
				_socket, &QWebSocket::deleteLater);
		_socket->close();
		_socket = nullptr;
	}

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
