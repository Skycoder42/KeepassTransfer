#include "qrencoder.h"
#include <QDebug>
using namespace CryptoPP;

QrEncoder::QrEncoder(QObject *parent) :
	IEncoder{parent}
{
	registerEncoder(EncryptedData::DataMode::QrCode, this);
}

DataEncryptor::ECCCurve QrEncoder::curve() const
{
	return _curve;
}

QString QrEncoder::publicKey() const
{
	try {
		if(!_privKey)
			return {};

		if(_pubKey.isNull()) {
			auto baData = encryptor()->serializePublicKey(_privKey);
			_pubKey = QString::fromUtf8(baData.toBase64());
		}

		return _pubKey;
	} catch(std::exception &e) {
		qCritical() << "Failed to obtain public key from private key with error:"
					<< e.what();
		return {};
	}
}

bool QrEncoder::isValid() const
{
	return _privKey;
}

void QrEncoder::classBegin()
{
	_blocked = true;
}

void QrEncoder::componentComplete()
{
	_blocked = false;
	recreateKeys();
}

void QrEncoder::recreateKeys()
{
	if(_blocked)
		return;

	try {
		//TODO make async
		_privKey.clear();
		_pubKey.clear();
		emit publicKeyChanged({});
		_privKey = encryptor()->generateAsymKey(rng(), _curve);
		emit publicKeyChanged({});
	} catch(std::exception &e) {
		qCritical() << "Failed to generate new private key with error:"
					<< e.what();
	}
}

void QrEncoder::setCurve(DataEncryptor::ECCCurve curve)
{
	if (_curve == curve)
		return;

	_curve = curve;
	emit curveChanged(_curve);
	recreateKeys();
}

QByteArray QrEncoder::decryptData(const QByteArray &keyInfo, const QByteArray &iv, const QByteArray &data) const
{
	auto secretKey = encryptor()->decryptSecretKey(rng(), keyInfo, _privKey);
	auto plainData = encryptor()->decryptSymmetric(data, secretKey, iv);
	return plainData;
}
