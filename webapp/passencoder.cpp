#include "passencoder.h"

PassEncoder::PassEncoder(QObject *parent) :
	IEncoder{parent}
{
	registerEncoder(EncryptedData::DataMode::Password, this);
}

QString PassEncoder::passphrase() const
{
	return _passphrase;
}

void PassEncoder::setPassphrase(QString passphrase)
{
	if (_passphrase == passphrase)
		return;

	_passphrase = std::move(passphrase);
	emit passphraseChanged(_passphrase);
}

void PassEncoder::generateRandom()
{
	QByteArray rndBlock{12, Qt::Uninitialized};
	rng().GenerateBlock(reinterpret_cast<CryptoPP::byte*>(rndBlock.data()),
						static_cast<size_t>(rndBlock.size()));
	setPassphrase(QString::fromUtf8(rndBlock.toBase64(QByteArray::OmitTrailingEquals)));
}

QByteArray PassEncoder::decryptData(const QByteArray &keyInfo, const QByteArray &iv, const QByteArray &data) const
{
	const auto secretKey = encryptor()->generateSecretKey(_passphrase, keyInfo);
	return encryptor()->decryptSymmetric(data, secretKey, iv);
}
