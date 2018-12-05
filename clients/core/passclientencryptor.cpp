#include "passclientencryptor.h"
using namespace CryptoPP;

PassClientEncryptor::PassClientEncryptor(DataEncryptor *encryptor, QObject *parent) :
	QObject{parent},
	_encryptor{encryptor}
{}

EncryptedData::DataMode PassClientEncryptor::mode() const
{
	return EncryptedData::DataMode::Password;
}

QUuid PassClientEncryptor::channelId() const
{
	return _channelId;
}

std::pair<SecByteBlock, QByteArray> PassClientEncryptor::obtainKeys(RandomNumberGenerator &rng)
{
	QByteArray salt{AES::MAX_KEYLENGTH, Qt::Uninitialized};
	rng.GenerateBlock(reinterpret_cast<byte*>(salt.data()),
					  static_cast<size_t>(salt.size()));
	const auto key = _encryptor->generateSecretKey(_passphrase, salt);
	return std::make_pair(key, salt);
}

QString PassClientEncryptor::passphrase() const
{
	return _passphrase;
}

void PassClientEncryptor::setChannelId(QUuid channelId)
{
	if (_channelId == channelId)
		return;

	_channelId = std::move(channelId);
	emit channelIdChanged(_channelId);
}

void PassClientEncryptor::setPassphrase(QString passphrase)
{
	if (_passphrase == passphrase)
		return;

	_passphrase = std::move(passphrase);
	emit passphraseChanged(_passphrase);
}
