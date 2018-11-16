#include <QRandomGenerator64>
#include <QDebug>
#include <cryptopp/aes.h>
#include <cryptopp/gcm.h>
#include <cryptopp/filters.h>
#include <cryptopp/secblock.h>
#include <cryptopp/randpool.h>
#include <cryptopp/osrng.h>
#include <iostream>
using namespace CryptoPP;
using namespace std;

void testCryptopp()
{
	try {
		AutoSeededRandomPool prng;

		SecByteBlock key(AES::DEFAULT_KEYLENGTH);
		prng.GenerateBlock(key, key.size());
		qDebug() << QByteArray::fromRawData(reinterpret_cast<char*>(key.data()), key.size()).toHex();

		SecByteBlock iv(AES::BLOCKSIZE);
		prng.GenerateBlock(iv, iv.size());
		qDebug() << QByteArray::fromRawData(reinterpret_cast<char*>(iv.data()), iv.size()).toHex();

		string pdata = "Authenticated Encryption";
		string cipher;
		string rpdata;

		GCM<AES>::Encryption e;
		e.SetKeyWithIV(key, key.size(), iv, sizeof(iv));
		StringSource (pdata, true,
			new AuthenticatedEncryptionFilter(e,
				new StringSink(cipher)
			) // AuthenticatedEncryptionFilter
		); // StringSource

		GCM<AES>::Decryption d;
		d.SetKeyWithIV(key, key.size(), iv, sizeof(iv));
		StringSource (cipher, true,
			new AuthenticatedDecryptionFilter(d,
				new StringSink(rpdata)
			)
		); // StringSource

		qDebug() << QByteArray::fromStdString(pdata)
				 << QByteArray::fromStdString(cipher).toHex()
				 << QByteArray::fromStdString(rpdata);
	} catch(exception &e) {
		qCritical() << e.what();
	}
}
