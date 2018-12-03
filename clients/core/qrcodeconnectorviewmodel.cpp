#include "qrcodeconnectorviewmodel.h"
#include "transferselectionviewmodel.h"

QrCodeConnectorViewModel::QrCodeConnectorViewModel(QObject *parent) :
	ViewModel{parent}
{}

QString QrCodeConnectorViewModel::qrData() const
{
	return _qrRawData;
}

bool QrCodeConnectorViewModel::isValid() const
{
	return _qrCryptor->qrData().isValid();
}

ClientTransferService *QrCodeConnectorViewModel::transferService() const
{
	return _transferService;
}

bool QrCodeConnectorViewModel::transfer()
{
	if(!isValid())
		return false;

	_transferService->sendCredentials(_qrCryptor, _credentials);
	return true;
}

void QrCodeConnectorViewModel::onInit(const QVariantHash &params)
{
	Q_ASSERT(_transferService);
	Q_ASSERT(_encryptor);
	_qrCryptor = new QrClientEncryptor{_encryptor, this};

	_credentials = params.value(TransferSelectionViewModel::paramCred).value<QList<Credential>>();
}

void QrCodeConnectorViewModel::setQrData(QString qrData)
{
	if (_qrRawData == qrData)
		return;

	_qrRawData = std::move(qrData);
	_qrCryptor->setQrData(KPTLib::decode<QrData>(_qrRawData.trimmed().toUtf8(), true));
	emit qrDataChanged({});
}
