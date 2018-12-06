#ifndef TRANSFERSERVER_H
#define TRANSFERSERVER_H

#include <QObject>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QUuid>
#include <QHash>
#include <QDeadlineTimer>
#include <QTimer>

#include <messages/errormessage.h>
#include <messages/appidentmessage.h>
#include <messages/clienttransfermessage.h>
#include <messages/appokmessage.h>

class TransferServer : public QObject
{
	Q_OBJECT

public:
	explicit TransferServer(qint64 timeout, QObject *parent = nullptr);

	bool startServer(const QString &serverName, int socket);
	bool startServer(const QString &serverName, bool localHostOnly, quint16 port);

private slots:
	void newConnection();

	void acceptError(QAbstractSocket::SocketError socketError);
	void serverError(QWebSocketProtocol::CloseCode closeCode);

	void onInvalidMessage(int typeId, QWebSocket *socket);
	void onInvalidMessage2(int typeId, QWebSocket *socket, QWebSocket *partnerSocket);

	void onAppIdent(const AppIdentMessage message, QWebSocket *socket);
	void onClientTransfer(ClientTransferMessage message, QWebSocket *clientSocket);
	void onAppOk(const AppOkMessage &message, QWebSocket *clientSocket);
	void onAppError(const ErrorMessage &message, QWebSocket *clientSocket);

	void timeout();

private:
	QWebSocketServer *_server = nullptr;
	QHash<QUuid, QWebSocket*> _appHash;

	QTimer *_timeoutTimer;
	const qint64 _timeout;
	QHash<QWebSocket*, std::pair<QDeadlineTimer, bool>> _timeouts;

	void setup();
	void setupCleanupConnections(QWebSocket *socket);

	bool verifyProtocolVersion(quint32 version, QWebSocket *socket);
	void closeOnTimeout(QWebSocket *socket, int timeout = 5000);
};

#endif // TRANSFERSERVER_H
