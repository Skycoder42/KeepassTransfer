#ifndef QRCODECONNECTORVIEWMODEL_H
#define QRCODECONNECTORVIEWMODEL_H

#include <QtCore/QUuid>
#include <QtMvvmCore/ViewModel>
#include <credential.h>
#include <qrdata.h>
#include "clienttransferservice.h"
#include "qrclientencryptor.h"

class QrCodeConnectorViewModel : public QtMvvm::ViewModel
{
	Q_OBJECT
	QTMVVM_CONTAINER_VM(KptRootViewModel)

	QTMVVM_INJECT_PROP(DataEncryptor*, encryptor, _encryptor)
	QTMVVM_INJECT_PROP(ClientTransferService*, transferService, _transferService)

	Q_PROPERTY(QString qrData READ qrData WRITE setQrData NOTIFY qrDataChanged)
	Q_PROPERTY(bool valid READ isValid NOTIFY qrDataChanged)

public:
	Q_INVOKABLE explicit QrCodeConnectorViewModel(QObject *parent = nullptr);

	QString qrData() const;
	bool isValid() const;

public slots:
	void transfer();

	void onInit(const QVariantHash &params) override;

	void setQrData(QString qrData);

signals:
	void qrDataChanged(QPrivateSignal);

private:
	DataEncryptor *_encryptor = nullptr;
	ClientTransferService *_transferService = nullptr;
	QrClientEncryptor *_qrCryptor = nullptr;

	QList<Credential> _credentials;
	QString _qrRawData;
};

#endif // QRCODECONNECTORVIEWMODEL_H
