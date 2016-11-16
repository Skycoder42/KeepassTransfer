#ifndef APPSERVER_H
#define APPSERVER_H

#include "clienthandler.h"

#include <QObject>
#include <QWebSocketServer>

class AppServer : public QObject
{
	Q_OBJECT
public:
	explicit AppServer(QObject *parent = nullptr);

	void registerWebClient(QString secret, ClientHandler *handler);
	void registerMobileClient(QString secret, ClientHandler *handler);

	ClientHandler *findWebClient(QString secret) const;
	ClientHandler *findMobileClient(QString secret) const;

public slots:
	bool start(int port);

private slots:
	void newConnection();

	void acceptError(QAbstractSocket::SocketError socketError);
	void serverError(QWebSocketProtocol::CloseCode closeCode);
	void sslErrors(const QList<QSslError> &errors);

private:
	QWebSocketServer *server;

	QHash<QString, ClientHandler*> webClients;
	QHash<QString, ClientHandler*> mobileClients;
};

#endif // APPSERVER_H
