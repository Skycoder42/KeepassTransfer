#include "qrclientencryptor.h"
using namespace CryptoPP;

QrClientEncryptor::QrClientEncryptor(DataEncryptor *encryptor, QObject *parent) :
	QObject{parent},
	_encryptor{encryptor}
{
	Q_ASSERT(_encryptor);
}

EncryptedData::DataMode QrClientEncryptor::mode() const
{
	return EncryptedData::DataMode::QrCode;
}

QUuid QrClientEncryptor::channelId() const
{
	return _qrData.channelId;
}

std::pair<SecByteBlock, QByteArray> QrClientEncryptor::obtainKeys(RandomNumberGenerator &rng)
{
	SecByteBlock secKey{AES::MAX_KEYLENGTH};
	rng.GenerateBlock(secKey, secKey.size());
	const auto pubKey = _encryptor->deserializePublicKey(rng, _qrData.pubKey);
	auto keyData = _encryptor->encryptSecretKey(rng, secKey, pubKey);
	return std::make_pair(std::move(secKey), std::move(keyData));
}

QrData QrClientEncryptor::qrData() const
{
	return _qrData;
}

void QrClientEncryptor::setQrData(QrData qrData)
{
	_qrData = std::move(qrData);
}
