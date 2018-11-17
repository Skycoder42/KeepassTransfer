#ifndef TRANSFERSERVER_H
#define TRANSFERSERVER_H

#include <QObject>
#include <QWebSocket>
#include <QWebSocketServer>

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

private:
	QWebSocketServer *_server = nullptr;

	void setup();
};

#endif // TRANSFERSERVER_H
