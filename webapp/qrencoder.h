#ifndef QRENCODER_H
#define QRENCODER_H

#include <QObject>
#include <QQmlParserStatus>
#include <iencoder.h>

class QrEncoder : public IEncoder, public QQmlParserStatus
{
	Q_OBJECT
	Q_INTERFACES(QQmlParserStatus)

	Q_PROPERTY(DataEncryptor::ECCCurve curve READ curve WRITE setCurve NOTIFY curveChanged)

	Q_PROPERTY(QString publicKey READ publicKey NOTIFY publicKeyChanged)
	Q_PROPERTY(bool valid READ isValid NOTIFY publicKeyChanged)

public:
	explicit QrEncoder(QObject *parent = nullptr);

	DataEncryptor::ECCCurve curve() const;
	QString publicKey() const;
	bool isValid() const;

	void classBegin() override;
	void componentComplete() override;

public slots:
	void recreateKeys();

	void setCurve(DataEncryptor::ECCCurve curve);

signals:
	void publicKeyChanged(QPrivateSignal);
	void curveChanged(DataEncryptor::ECCCurve curve);

protected:
	QByteArray decryptData(const QByteArray &keyInfo, const QByteArray &iv, const QByteArray &data) const override;

private:
	DataEncryptor::ECCCurve _curve = DataEncryptor::ECCCurve::secp256r1;

	bool _blocked = false;
	QSharedPointer<CryptoPP::PKCS8PrivateKey> _privKey;
	mutable QString _pubKey;
};

#endif // QRENCODER_H
