#ifndef SERVERCONNECTOR_H
#define SERVERCONNECTOR_H

#include <QObject>
#include <QWebSocket>

#include <credential.h>
#include <messages/serveridentmessage.h>
#include <messages/errormessage.h>
#include <messages/servertransfermessage.h>

class ServerConnector : public QObject
{
	Q_OBJECT

	Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
	Q_PROPERTY(QUuid appId READ appId NOTIFY appIdChanged)
	Q_PROPERTY(QString appIdStr READ appIdStr NOTIFY appIdChanged)

public:
	explicit ServerConnector(QUrl url,
							 QObject *parent = nullptr);

	bool isConnected() const;
	QUuid appId() const;
	QString appIdStr() const;

signals:
	void credentialsReceived(const QVariantList &receivedCreds, const QString &entryTitle);
	void serverError(const QString &message);

	void connectedChanged(bool connected, QPrivateSignal);
	void appIdChanged(QPrivateSignal);

private slots:
	void connected();
	void disconnected();
	void error(QAbstractSocket::SocketError error);
	void binaryMessageReceived(const QByteArray &message);

	void recreateSocket();

private:
	const QUrl _serverUrl;
	QWebSocket *_socket = nullptr;
	QUuid _appId;

	int _retryTimeout = 1000;

	void onServerIdent(const ServerIdentMessage message);
	void onError(const ErrorMessage &message);
	void onServerTransfer(const ServerTransferMessage &message);
	void onFallback(int typeId);
};

#endif // SERVERCONNECTOR_H
