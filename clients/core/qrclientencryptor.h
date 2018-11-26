#ifndef QRCLIENTENCRYPTOR_H
#define QRCLIENTENCRYPTOR_H

#include <QObject>
#include <qrdata.h>
#include <dataencryptor.h>
#include "clienttransferservice.h"

class QrClientEncryptor : public QObject, public IClientEncryptor
{
	Q_OBJECT
	Q_INTERFACES(IClientEncryptor)

	Q_PROPERTY(QrData qrData READ qrData WRITE setQrData)

public:
	explicit QrClientEncryptor(DataEncryptor *encryptor, QObject *parent = nullptr);

	EncryptedData::DataMode mode() const override;
	QUuid channelId() const override;
	std::pair<CryptoPP::SecByteBlock, QByteArray> obtainKeys(CryptoPP::RandomNumberGenerator &rng) override;

	QrData qrData() const;

public slots:
	void setQrData(QrData qrData);

private:
	const DataEncryptor *_encryptor;
	QrData _qrData;
};

#endif // QRCLIENTENCRYPTOR_H
