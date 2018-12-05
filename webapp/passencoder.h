#ifndef PASSENCODER_H
#define PASSENCODER_H

#include "iencoder.h"
#include <QQmlParserStatus>

class PassEncoder : public IEncoder
{
	Q_OBJECT

	Q_PROPERTY(QString passphrase READ passphrase WRITE setPassphrase NOTIFY passphraseChanged)

public:
	explicit PassEncoder(QObject *parent = nullptr);

	QString passphrase() const;

public slots:
	void setPassphrase(QString passphrase);

	void generateRandom();

signals:
	void passphraseChanged(const QString &passphrase);

protected:
	QByteArray decryptData(const QByteArray &keyInfo, const QByteArray &iv, const QByteArray &data) const override;

private:
	QString _passphrase;
};

#endif // PASSENCODER_H
