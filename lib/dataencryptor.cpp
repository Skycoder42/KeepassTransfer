#include "dataencryptor.h"

#include <cryptopp/aes.h>
#include <cryptopp/gcm.h>
#include <cryptopp/filters.h>
#include <cryptopp/eccrypto.h>
#include <cryptopp/sha3.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/pwdbased.h>
#include <qiodevicesource.h>
#include <qiodevicesink.h>
using namespace CryptoPP;

namespace {

class SecByteBlockSink : public Bufferless<Sink>
{
public:
	SecByteBlockSink(SecByteBlock& sbb) :
		m_sbb{sbb}
	{}

	size_t Put2(const byte *inString, size_t length, int /*messageEnd*/, bool /*blocking*/) {
		if(!inString || !length)
			return length;
		m_sbb += SecByteBlock{inString, length};
		return 0;
	}

private:
	SecByteBlock &m_sbb;
};

}

using EciesScheme = CryptoPP::ECIES<CryptoPP::ECP, CryptoPP::SHA3_512>;

DataEncryptor::DataEncryptor(QObject *parent) :
	QObject{parent}
{}

QByteArray DataEncryptor::encryptSymmetric(const QByteArray &plain, const CryptoPP::SecByteBlock &key, const QByteArray &iv) const
{
	GCM<AES>::Encryption encryptor;
	encryptor.SetKeyWithIV(key,
						   key.size(),
						   reinterpret_cast<const byte*>(iv.constData()),
						   static_cast<size_t>(iv.size()));

	QByteArray cipher;
	QByteArraySource{plain, true,
		new AuthenticatedEncryptionFilter{encryptor,
			new QByteArraySink{cipher}
		}
	};

	return cipher;
}

QByteArray DataEncryptor::decryptSymmetric(const QByteArray &cipher, const SecByteBlock &key, const QByteArray &iv) const
{
	GCM<AES>::Decryption decryptor;
	decryptor.SetKeyWithIV(key,
						   key.size(),
						   reinterpret_cast<const byte*>(iv.constData()),
						   static_cast<size_t>(iv.size()));

	QByteArray plain;
	QByteArraySource{cipher, true,
		new AuthenticatedDecryptionFilter{decryptor,
			new QByteArraySink{plain}
		}
	};

	return plain;
}

SecByteBlock DataEncryptor::generateSecretKey(const QString &password, const QByteArray &salt) const
{
	const auto pwData = password.toUtf8();
	SecByteBlock secKey{AES::MAX_KEYLENGTH};

	PKCS5_PBKDF2_HMAC<SHA3_256> keydev;
	keydev.DeriveKey(secKey, secKey.size(),
					 0x00,
					 reinterpret_cast<const byte*>(pwData.constData()),
					 static_cast<size_t>(pwData.size()),
					 reinterpret_cast<const byte*>(salt.constData()),
					 static_cast<size_t>(salt.size()),
					 4200);

	return secKey;
}

QSharedPointer<PKCS8PrivateKey> DataEncryptor::generateAsymKey(RandomNumberGenerator &rng, ECCCurve curve) const
{
	QSharedPointer<EciesScheme::PrivateKey> privKey;
	do {
		privKey = QSharedPointer<EciesScheme::PrivateKey>::create();
		privKey->Initialize(rng, curveOid(curve));
	} while(!privKey->Validate(rng, 3));
	return privKey;
}

QByteArray DataEncryptor::serializePublicKey(const X509PublicKey &pubKey) const
{
	QByteArray data;
	QByteArraySink sink{data};
	pubKey.Save(sink);
	return data;
}

QSharedPointer<X509PublicKey> DataEncryptor::deserializePublicKey(RandomNumberGenerator &rng, const QByteArray &data) const
{
	auto pubKey = QSharedPointer<EciesScheme::PublicKey>::create();
	QByteArraySource source{data, true};
	pubKey->Load(source);
	if(!pubKey->Validate(rng, 3))
		throw Exception{Exception::INVALID_DATA_FORMAT, "Key failed validation"};
	return pubKey;
}

QByteArray DataEncryptor::encryptSecretKey(RandomNumberGenerator &rng, const SecByteBlock &secKey, const X509PublicKey &pubKey) const
{
	EciesScheme::Encryptor encryptor{pubKey};

	QByteArray cipher;
	StringSource{secKey, secKey.size(), true,
		new PK_EncryptorFilter{rng, encryptor,
			new QByteArraySink{cipher}
		}
	};
	return cipher;
}

SecByteBlock DataEncryptor::decryptSecretKey(RandomNumberGenerator &rng, const QByteArray &data, const PKCS8PrivateKey &privKey) const
{
	EciesScheme::Decryptor decryptor{privKey};

	SecByteBlock secKey;
	QByteArraySource{data, true,
		new PK_DecryptorFilter{rng, decryptor,
			new SecByteBlockSink{secKey}
		}
	};

	return secKey;
}

OID DataEncryptor::curveOid(DataEncryptor::ECCCurve curve) const
{
	switch(curve) {
	case ECCCurve::secp128r1:
		return ASN1::secp128r1();
	case ECCCurve::secp256r1:
		return ASN1::secp256r1();
	case ECCCurve::secp128r2:
		return ASN1::secp128r2();
	case ECCCurve::secp256k1:
		return ASN1::secp256k1();
	case ECCCurve::brainpoolP256r1:
		return ASN1::brainpoolP256r1();
	case ECCCurve::brainpoolP512r1:
		return ASN1::brainpoolP512r1();
	default:
		Q_UNREACHABLE();
		break;
	}
}
