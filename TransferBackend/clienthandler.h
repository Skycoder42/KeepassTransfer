#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QJsonValue>
#include <QObject>
#include <QWebSocket>

class AppServer;
class ClientHandler : public QObject
{
	Q_OBJECT

public:
	enum HandlerMode {
		Undefined,
		WebClient,
		MobileClient
	};
	Q_ENUM(HandlerMode)

	explicit ClientHandler(QWebSocket *socket, AppServer *parent = nullptr);

public slots:
	void sendData(QJsonValue data);

private slots:
	void textMessageReceived(const QString &message);

	void error(QAbstractSocket::SocketError error);
	void sslErrors(const QList<QSslError> &errors);

private:
	AppServer *appServer();
	QWebSocket *socket;
	HandlerMode mode;
	QString mySecret;

	void readInitialMobileData(QJsonObject secretData);
	void readInitialWebData(QJsonObject secretData);
	void readWebStatus(QJsonObject status);
};

#endif // CLIENTHANDLER_H
