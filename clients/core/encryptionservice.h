#ifndef ENCRYPTIONSERVICE_H
#define ENCRYPTIONSERVICE_H

#include <QObject>
#include <credential.h>
#include <encrypteddata.h>
#include <dataencryptor.h>
#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>

class IClientEncryptor : public QObject
{
	Q_OBJECT

public:
	explicit IClientEncryptor(QObject *parent = nullptr);

	virtual QString name() const = 0;
	virtual EncryptedData::DataMode mode() const = 0;
	virtual std::pair<CryptoPP::SecByteBlock, QByteArray> obtainKeys(CryptoPP::RandomNumberGenerator &rng) = 0;
};

class EncryptionService : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE explicit EncryptionService(QObject *parent = nullptr);

	Q_INVOKABLE EncryptedData encrypt(IClientEncryptor *clientCrypt, const QList<Credential> &credentials);

private:
	DataEncryptor *_encryptor;
	CryptoPP::AutoSeededRandomPool _rng;
};

#define IClientEncryptorIid "de.skycoder42.kpt.IClientEncryptor"
Q_DECLARE_INTERFACE(IClientEncryptor, IClientEncryptorIid)

#endif // ENCRYPTIONSERVICE_H
