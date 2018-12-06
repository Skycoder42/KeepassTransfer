#ifndef QRCODECONNECTORVIEWMODEL_H
#define QRCODECONNECTORVIEWMODEL_H

#include <QtCore/QUuid>
#include <QtMvvmCore/ViewModel>
#include <credential.h>
#include <qrdata.h>
#include "clienttransferservice.h"
#include "qrclientencryptor.h"
#include "settings.h"

class QrCodeConnectorViewModel : public QtMvvm::ViewModel
{
	Q_OBJECT
	QTMVVM_CONTAINER_VM(KptRootViewModel)

	QTMVVM_INJECT_PROP(DataEncryptor*, encryptor, _encryptor)
	QTMVVM_INJECT_PROP(Settings*, settings, _settings)
	QTMVVM_INJECT_PROP(ClientTransferService*, transferService, _transferService)

	Q_PROPERTY(QString qrData READ qrData WRITE setQrData NOTIFY qrDataChanged)
	Q_PROPERTY(bool valid READ isValid NOTIFY qrDataChanged)
	Q_PROPERTY(bool pasteTransfer READ doPasteTransfer NOTIFY qrDataChanged)

public:
	Q_INVOKABLE explicit QrCodeConnectorViewModel(QObject *parent = nullptr);

	QString qrData() const;
	bool isValid() const;
	bool doPasteTransfer() const;

	ClientTransferService* transferService() const;

public slots:
	bool transfer();

	void onInit(const QVariantHash &params) override;

	void setQrData(QString qrData);

signals:
	void qrDataChanged(QPrivateSignal);

private slots:
	void transferDone();

private:
	DataEncryptor *_encryptor = nullptr;
	Settings *_settings = nullptr;
	ClientTransferService *_transferService = nullptr;
	QrClientEncryptor *_qrCryptor = nullptr;

	QList<Credential> _credentials;
	QString _qrRawData;
};

#endif // QRCODECONNECTORVIEWMODEL_H
