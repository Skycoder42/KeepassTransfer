#ifndef PASSCLIENTENCRYPTOR_H
#define PASSCLIENTENCRYPTOR_H

#include <QObject>
#include "clienttransferservice.h"

class PassClientEncryptor : public QObject, public IClientEncryptor
{
	Q_OBJECT
	Q_INTERFACES(IClientEncryptor)

	Q_PROPERTY(QUuid channelId READ channelId WRITE setChannelId NOTIFY channelIdChanged)
	Q_PROPERTY(QString passphrase READ passphrase WRITE setPassphrase NOTIFY passphraseChanged)

public:
	explicit PassClientEncryptor(DataEncryptor *encryptor, QObject *parent = nullptr);

	EncryptedData::DataMode mode() const override;
	QUuid channelId() const override;
	std::pair<CryptoPP::SecByteBlock, QByteArray> obtainKeys(CryptoPP::RandomNumberGenerator &rng) override;

	QString passphrase() const;

public slots:
	void setChannelId(QUuid channelId);
	void setPassphrase(QString passphrase);

signals:
	void channelIdChanged(QUuid channelId);
	void passphraseChanged(const QString &passphrase);

private:
	DataEncryptor *_encryptor;
	QUuid _channelId;
	QString _passphrase;
};

#endif // PASSCLIENTENCRYPTOR_H
