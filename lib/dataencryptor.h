#ifndef DATAENCRYPTOR_H
#define DATAENCRYPTOR_H

#include <QObject>
#include <QSharedPointer>

#include <cryptopp/secblock.h>
#include <cryptopp/pubkey.h>
#include <cryptopp/oids.h>
#include <cryptopp/rng.h>

class DataEncryptor : public QObject
{
	Q_OBJECT

public:
	enum class ECCCurve {
		secp128r1,
		secp256r1,
		secp128r2,
		secp256k1,
		brainpoolP256r1,
		brainpoolP512r1
	};
	Q_ENUM(ECCCurve)

	explicit DataEncryptor(QObject *parent = nullptr);

	// symmetric encryption
	QByteArray encryptSymmetric(const QByteArray &plain,
								const CryptoPP::SecByteBlock &key,
								const QByteArray &iv) const;
	QByteArray decryptSymmetric(const QByteArray &cipher,
								const CryptoPP::SecByteBlock &key,
								const QByteArray &iv) const;

	// pw-based key generation
	CryptoPP::SecByteBlock generateSecretKey(const QString &password, const QByteArray &salt) const;

	// asymmetric key encryption
	QSharedPointer<CryptoPP::PKCS8PrivateKey> generateAsymKey(CryptoPP::RandomNumberGenerator &rng,
															  ECCCurve curve) const;

	QByteArray serializePublicKey(const CryptoPP::X509PublicKey &pubKey) const;
	inline QByteArray serializePublicKey(const QSharedPointer<CryptoPP::X509PublicKey> &pubKey) const {
		return serializePublicKey(*(pubKey.data()));
	}
	QByteArray serializePublicKey(const CryptoPP::PKCS8PrivateKey &privKey) const;
	inline QByteArray serializePublicKey(const QSharedPointer<CryptoPP::PKCS8PrivateKey> &privKey) const {
		return serializePublicKey(*(privKey.data()));
	}
	QSharedPointer<CryptoPP::X509PublicKey> deserializePublicKey(CryptoPP::RandomNumberGenerator &rng,
																 const QByteArray &data) const;

	QByteArray encryptSecretKey(CryptoPP::RandomNumberGenerator &rng,
								const CryptoPP::SecByteBlock &secKey,
								const CryptoPP::X509PublicKey &pubKey) const;
	inline QByteArray encryptSecretKey(CryptoPP::RandomNumberGenerator &rng,
								const CryptoPP::SecByteBlock &secKey,
								const QSharedPointer<CryptoPP::X509PublicKey> &pubKey) const {
		return encryptSecretKey(rng, secKey, *(pubKey.data()));
	}

	CryptoPP::SecByteBlock decryptSecretKey(CryptoPP::RandomNumberGenerator &rng,
											const QByteArray &data,
											const CryptoPP::PKCS8PrivateKey &privKey) const;
	inline CryptoPP::SecByteBlock decryptSecretKey(CryptoPP::RandomNumberGenerator &rng,
											const QByteArray &data,
											const QSharedPointer<CryptoPP::PKCS8PrivateKey> &privKey) const {
		return decryptSecretKey(rng, data, *(privKey.data()));
	}

private:
	CryptoPP::OID curveOid(ECCCurve curve) const;
};

#endif // DATAENCRYPTOR_H
