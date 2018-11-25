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
	return _qrData.isValid();
}

void QrCodeConnectorViewModel::onInit(const QVariantHash &params)
{
	Q_ASSERT(_transferService);
	_credentials = params.value(TransferSelectionViewModel::paramCred).value<QList<Credential>>();
}

void QrCodeConnectorViewModel::setQrData(QString qrData)
{
	if (_qrRawData == qrData)
		return;

	_qrRawData = std::move(qrData);
	_qrData = KPTLib::decode<QrData>(_qrRawData.toUtf8(), true);
	emit qrDataChanged({});
}
