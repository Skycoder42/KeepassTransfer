#include "qrencoder.h"
#include <QDebug>
#include <kptlib.h>
#include <qrdata.h>
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

QUuid QrEncoder::channelId() const
{
	return _channelId;
}

QString QrEncoder::qrData() const
{
	try {
		if(!_privKey)
			return {};

		if(_pubKey.isNull()) {
			_qrData.clear();
			_pubKey = encryptor()->serializePublicKey(_privKey);
		}

		if(_qrData.isNull())
			_qrData = QString::fromUtf8(KPTLib::encode(QrData{_channelId, _pubKey}, true));

		return _qrData;
	} catch(std::exception &e) {
		qCritical() << "Failed to obtain public key from private key with error:"
					<< e.what();
		return {};
	}
}

bool QrEncoder::isValid() const
{
	return _privKey && !_channelId.isNull();
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
		_qrData.clear();
		emit qrDataChanged({});
		_privKey = encryptor()->generateAsymKey(rng(), _curve);
		emit qrDataChanged({});
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

void QrEncoder::setChannelId(QUuid channelId)
{
	if (_channelId == channelId)
		return;

	_channelId = std::move(channelId);
	_qrData.clear();
	emit qrDataChanged({});
}

QByteArray QrEncoder::decryptData(const QByteArray &keyInfo, const QByteArray &iv, const QByteArray &data) const
{
	auto secretKey = encryptor()->decryptSecretKey(rng(), keyInfo, _privKey);
	auto plainData = encryptor()->decryptSymmetric(data, secretKey, iv);
	return plainData;
}
