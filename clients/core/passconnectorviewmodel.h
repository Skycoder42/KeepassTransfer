#ifndef PASSCONNECTORVIEWMODEL_H
#define PASSCONNECTORVIEWMODEL_H

#include <QtCore/QUuid>
#include <QtMvvmCore/ViewModel>
#include "passclientencryptor.h"

class PassConnectorViewModel : public QtMvvm::ViewModel
{
	Q_OBJECT
	QTMVVM_CONTAINER_VM(KptRootViewModel)

	QTMVVM_INJECT_PROP(DataEncryptor*, encryptor, _encryptor)
	QTMVVM_INJECT_PROP(Settings*, settings, _settings)
	QTMVVM_INJECT_PROP(ClientTransferService*, transferService, _transferService)

	Q_PROPERTY(QUuid channelId READ channelId WRITE setChannelId NOTIFY channelIdChanged)
	Q_PROPERTY(QString passphrase READ passphrase WRITE setPassphrase NOTIFY passphraseChanged)
	Q_PROPERTY(bool valid READ isValid NOTIFY channelIdChanged)

public:
	Q_INVOKABLE explicit PassConnectorViewModel(QObject *parent = nullptr);

	QUuid channelId() const;
	QString passphrase() const;
	bool isValid() const;

public slots:
	bool transfer();

	void onInit(const QVariantHash &params) override;

	void setChannelId(QUuid channelId);
	void setPassphrase(QString passphrase);

signals:
	void channelIdChanged();
	void passphraseChanged(const QString &passphrase);

private slots:
	void transferDone();

private:
	DataEncryptor *_encryptor = nullptr;
	Settings *_settings = nullptr;
	ClientTransferService *_transferService = nullptr;
	PassClientEncryptor *_passCryptor = nullptr;

	QList<Credential> _credentials;
};

#endif // PASSCONNECTORVIEWMODEL_H
