#ifndef QRENCODER_H
#define QRENCODER_H

#include <QObject>
#include <QUuid>
#include <QQmlParserStatus>
#include <iencoder.h>

class QrEncoder : public IEncoder, public QQmlParserStatus
{
	Q_OBJECT
	Q_INTERFACES(QQmlParserStatus)

	Q_PROPERTY(DataEncryptor::ECCCurve curve READ curve WRITE setCurve NOTIFY curveChanged)

	Q_PROPERTY(QUuid channelId READ channelId WRITE setChannelId NOTIFY qrDataChanged)
	Q_PROPERTY(QString qrData READ qrData NOTIFY qrDataChanged)
	Q_PROPERTY(bool valid READ isValid NOTIFY qrDataChanged)

public:
	explicit QrEncoder(QObject *parent = nullptr);

	DataEncryptor::ECCCurve curve() const;
	QUuid channelId() const;
	QString qrData() const;
	bool isValid() const;

	void classBegin() override;
	void componentComplete() override;

public slots:
	void recreateKeys();

	void setCurve(DataEncryptor::ECCCurve curve);
	void setChannelId(QUuid channelId);

signals:
	void qrDataChanged(QPrivateSignal);
	void curveChanged(DataEncryptor::ECCCurve curve);

protected:
	QByteArray decryptData(const QByteArray &keyInfo, const QByteArray &iv, const QByteArray &data) const override;

private:
	DataEncryptor::ECCCurve _curve = DataEncryptor::ECCCurve::secp256r1;

	bool _blocked = false;
	QSharedPointer<CryptoPP::PKCS8PrivateKey> _privKey;
	QUuid _channelId;

	mutable QByteArray _pubKey;
	mutable QString _qrData;
};

#endif // QRENCODER_H
