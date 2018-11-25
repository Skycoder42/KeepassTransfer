#ifndef IENCODER_H
#define IENCODER_H

#include <QObject>
#include <QPointer>
#include <QHash>

#include <cryptopp/osrng.h>

#include <encrypteddata.h>
#include <dataencryptor.h>
#include <credential.h>

class IEncoder : public QObject
{
	Q_OBJECT

public:
	explicit IEncoder(QObject *parent = nullptr);

	static QList<Credential> decodeData(const EncryptedData &data);

protected:
	static void registerEncoder(EncryptedData::DataMode mode, IEncoder *encoder);

	CryptoPP::RandomNumberGenerator &rng() const;
	const DataEncryptor *encryptor() const;

	virtual QByteArray decryptData(const QByteArray &keyInfo,
								   const QByteArray &iv,
								   const QByteArray &data) const = 0;

private:
	static QHash<EncryptedData::DataMode, QPointer<IEncoder>> _encoders;

	mutable CryptoPP::AutoSeededRandomPool _rng;
	DataEncryptor *_encryptor;
};

#endif // IENCODER_H
