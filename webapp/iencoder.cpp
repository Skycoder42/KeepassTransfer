#include "iencoder.h"
#include <kptlib.h>
#include <QDebug>
using namespace CryptoPP;

QHash<EncryptedData::DataMode, QPointer<IEncoder>> IEncoder::_encoders;

IEncoder::IEncoder(QObject *parent) :
	QObject{parent},
	_encryptor{new DataEncryptor{this}}
{}

QList<Credential> IEncoder::decodeData(const EncryptedData &data)
{
	try {
		auto encoder = _encoders.value(data.mode);
		if(!encoder) {
			qCritical() << "Unable to find a decoder for mode:" << data.mode;
			return {};
		}

		const auto rawData = encoder->decryptData(data.keyInfo, data.iv, data.data);
		return KPTLib::decode<QList<Credential>>(rawData);
	} catch(std::exception &e) {
		qCritical() << "Failed to decrypt data with error:" << e.what();
		return {};
	}
}

void IEncoder::registerEncoder(EncryptedData::DataMode mode, IEncoder *encoder)
{
	Q_ASSERT_X(!_encoders.value(mode), Q_FUNC_INFO, "Only one encoder can be registered for each mode at a time");
	_encoders.insert(mode, encoder);
}

RandomNumberGenerator &IEncoder::rng() const
{
	return _rng;
}

const DataEncryptor *IEncoder::encryptor() const
{
	return _encryptor;
}
