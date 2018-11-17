#ifndef TRANSFERSERVER_H
#define TRANSFERSERVER_H

#include <QObject>
#include <QWebSocket>
#include <QWebSocketServer>

#include <messages/appidentmessage.h>

class TransferServer : public QObject
{
	Q_OBJECT

public:
	explicit TransferServer(QObject *parent = nullptr);

	bool startServer(const QString &serverName, int socket);
	bool startServer(const QString &serverName, bool localHostOnly, quint16 port);

private slots:
	void newConnection();

	void acceptError(QAbstractSocket::SocketError socketError);
	void serverError(QWebSocketProtocol::CloseCode closeCode);

	void onInvalidMessage(int typeId, QWebSocket *socket);

	void onAppIdent(const AppIdentMessage &message, QWebSocket *socket);

private:
	QWebSocketServer *_server = nullptr;

	void setup();
	void setupCleanupConnections(QWebSocket *socket);
};

#endif // TRANSFERSERVER_H
