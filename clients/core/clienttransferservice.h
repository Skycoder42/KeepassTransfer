#ifndef CLIENTTRANSFERSERVICE_H
#define CLIENTTRANSFERSERVICE_H

#include <QtCore/QObject>
#include <QtCore/QThreadStorage>
#include <QtCore/QFutureWatcher>
#include <QtCore/QPointer>
#include <QtWebSockets/QWebSocket>
#include <QtMvvmCore/ProgressControl>
#include <QtMvvmCore/Injection>
#include <credential.h>
#include <encrypteddata.h>
#include <dataencryptor.h>
#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>
#include <messages/serverokmessage.h>
#include <messages/errormessage.h>

class IClientEncryptor
{
	Q_DISABLE_COPY(IClientEncryptor)
public:
	IClientEncryptor();
	virtual ~IClientEncryptor();

	virtual EncryptedData::DataMode mode() const = 0;
	virtual QUuid channelId() const = 0;
	virtual std::pair<CryptoPP::SecByteBlock, QByteArray> obtainKeys(CryptoPP::RandomNumberGenerator &rng) = 0;
};

class ClientTransferService : public QObject
{
	Q_OBJECT
	QTMVVM_INJECT_PROP(DataEncryptor*, encryptor, _encryptor)

public:
	Q_INVOKABLE explicit ClientTransferService(QObject *parent = nullptr);

public slots:
	void sendCredentials(IClientEncryptor *clientCrypt,
						 const QList<Credential> &credentials);

private slots:
	void connected();
	void disconnected();
	void binaryMessageReceived(const QByteArray &message);
	void onSocketError();

	void cryptDataReady();

private:
	static QThreadStorage<CryptoPP::AutoSeededRandomPool> _rngs;

	DataEncryptor *_encryptor = nullptr;

	QWebSocket *_currentSocket = nullptr;
	QPointer<QtMvvm::ProgressControl> _currentControl = nullptr;
	QFutureWatcher<EncryptedData> *_cryptWatcher = nullptr;
	IClientEncryptor *_currentCrypt = nullptr;
	bool _doSend = false;

	EncryptedData encrypt(IClientEncryptor *clientCrypt, const QList<Credential> &credentials);

	void onServerOk(const ServerOkMessage message);
	void onError(const ErrorMessage &message);
	void onFallback(int typeId);

	void sendData();
};

#define IClientEncryptorIid "de.skycoder42.kpt.IClientEncryptor"
Q_DECLARE_INTERFACE(IClientEncryptor, IClientEncryptorIid)

#endif // CLIENTTRANSFERSERVICE_H
