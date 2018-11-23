#include "encryptionservice.h"
using namespace CryptoPP;

EncryptionService::EncryptionService(QObject *parent) :
	QObject{parent},
	_encryptor{new DataEncryptor{this}}
{}

EncryptedData EncryptionService::encrypt(IClientEncryptor *clientCrypt, const QList<Credential> &credentials)
{
	QByteArray plain;
	QDataStream stream{&plain, QIODevice::WriteOnly};
	stream << credentials;

	QByteArray iv{AES::BLOCKSIZE, Qt::Uninitialized};
	_rng.GenerateBlock(reinterpret_cast<byte*>(iv.data()), AES::BLOCKSIZE);
	_rng.DiscardBytes(qChecksum(plain.constData(), static_cast<uint>(plain.size())));
	auto keys = clientCrypt->obtainKeys(_rng);
	auto cipher = _encryptor->encryptSymmetric({}, keys.first, iv);

	return EncryptedData {
		clientCrypt->mode(),
		std::move(keys.second),
		std::move(iv),
		std::move(cipher)
	};
}
