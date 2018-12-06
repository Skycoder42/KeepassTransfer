#ifndef PASSCONNECTORVIEWMODEL_H
#define PASSCONNECTORVIEWMODEL_H

#include <QtCore/QUuid>
#include <QtGui/QRegularExpressionValidator>
#include <QtMvvmCore/ViewModel>
#include <cryptopp/osrng.h>
#include "passclientencryptor.h"

class PassConnectorViewModel : public QtMvvm::ViewModel
{
	Q_OBJECT
	QTMVVM_CONTAINER_VM(KptRootViewModel)

	QTMVVM_INJECT_PROP(DataEncryptor*, encryptor, _encryptor)
	QTMVVM_INJECT_PROP(Settings*, settings, _settings)
	QTMVVM_INJECT_PROP(ClientTransferService*, transferService, _transferService)

	Q_PROPERTY(QRegularExpressionValidator* channelIdValidator READ channelIdValidator CONSTANT)

	Q_PROPERTY(QString channelIdStr READ channelIdStr WRITE setChannelIdStr NOTIFY channelIdChanged)
	Q_PROPERTY(QUuid channelId READ channelId WRITE setChannelId NOTIFY channelIdChanged)
	Q_PROPERTY(QString passphrase READ passphrase WRITE setPassphrase NOTIFY passphraseChanged)
	Q_PROPERTY(bool valid READ isValid NOTIFY channelIdChanged)

public:
	Q_INVOKABLE explicit PassConnectorViewModel(QObject *parent = nullptr);

	QRegularExpressionValidator* channelIdValidator() const;
	QUuid channelId() const;
	QString channelIdStr() const;
	QString passphrase() const;
	bool isValid() const;

public slots:
	bool transfer();
	void generateRandomPassphrase();

	void onInit(const QVariantHash &params) override;

	void setChannelId(QUuid channelId);
	void setChannelIdStr(QString channelIdStr);
	void setPassphrase(QString passphrase);

signals:
	void channelIdChanged();
	void passphraseChanged(const QString &passphrase);

private slots:
	void transferDone();

private:
	CryptoPP::AutoSeededRandomPool _rng;

	DataEncryptor *_encryptor = nullptr;
	Settings *_settings = nullptr;
	ClientTransferService *_transferService = nullptr;
	PassClientEncryptor *_passCryptor = nullptr;
	QRegularExpressionValidator* _channelIdValidator;

	QList<Credential> _credentials;
};

#endif // PASSCONNECTORVIEWMODEL_H
