#ifndef CLIENTTRANSFERSERVICE_H
#define CLIENTTRANSFERSERVICE_H

#include <QtCore/QObject>
#include <QtCore/QThreadStorage>
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

private:
	static QThreadStorage<CryptoPP::AutoSeededRandomPool> _rngs;

	DataEncryptor *_encryptor = nullptr;

	EncryptedData encrypt(IClientEncryptor *clientCrypt, const QList<Credential> &credentials);

	void onServerOk(const ServerOkMessage message, QtMvvm::ProgressControl *control);
	void onError(const ErrorMessage &message, QtMvvm::ProgressControl *control);
	void onFallback(int typeId, QtMvvm::ProgressControl *control);
	void onSocketError(QWebSocket *socket, QtMvvm::ProgressControl *control);
};

#define IClientEncryptorIid "de.skycoder42.kpt.IClientEncryptor"
Q_DECLARE_INTERFACE(IClientEncryptor, IClientEncryptorIid)

#endif // CLIENTTRANSFERSERVICE_H
