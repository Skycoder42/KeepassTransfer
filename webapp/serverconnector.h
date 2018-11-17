#ifndef SERVERCONNECTOR_H
#define SERVERCONNECTOR_H

#include <QObject>
#include <QWebSocket>

class ServerConnector : public QObject
{
	Q_OBJECT

public:
	explicit ServerConnector(QUrl url,
							 QObject *parent = nullptr);

private slots:
	void connected();
	void disconnected();
	void error(QAbstractSocket::SocketError error);
	void binaryMessageReceived(const QByteArray &message);

	void recreateSocket();

private:
	const QUrl _serverUrl;
	QWebSocket *_socket = nullptr;

	int _retryTimeout = 1000;
};

#endif // SERVERCONNECTOR_H
