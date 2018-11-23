#include "transferselectionviewmodel.h"
#include <QDebug>

const QString TransferSelectionViewModel::paramCred = QStringLiteral("credentials");

QVariantHash TransferSelectionViewModel::params(const QList<Credential> &creds)
{
	return {
		{paramCred, QVariant::fromValue(creds)}
	};
}

TransferSelectionViewModel::TransferSelectionViewModel(QObject *parent) :
	ViewModel{parent},
	_modeModel{new QStandardItemModel{0, 1, this}}
{
	auto qrItem = new QStandardItem{tr("QR-Code")};
	qrItem->setToolTip(tr("Take a foto of a QR-Code with key data."));
	_modeModel->appendRow(qrItem);

	auto pubItem = new QStandardItem{tr("Public Key")};
	pubItem->setToolTip(tr("Enter a public key received from the target."));
	_modeModel->appendRow(pubItem);

	auto passItem = new QStandardItem{tr("Passphrase")};
	passItem->setToolTip(tr("Choose a password and enter it here and on the target."));
	_modeModel->appendRow(passItem);
}

QStandardItemModel *TransferSelectionViewModel::modeModel() const
{
	return _modeModel;
}

void TransferSelectionViewModel::startTransfer(int index)
{
	switch (index) {
	case 0: //qrItem
		qDebug() << "starting qrItem";
		break;
	case 1: //pubItem
		qDebug() << "starting pubItem";
		break;
	case 2: //passItem
		qDebug() << "starting passItem";
		break;
	default:
		Q_UNREACHABLE();
	}
}

void TransferSelectionViewModel::startTransfer(const QModelIndex &index)
{
	startTransfer(index.row());
}

void TransferSelectionViewModel::onInit(const QVariantHash &params)
{
	_credentials = params.value(paramCred).value<QList<Credential>>();
	qDebug() << _credentials.size();
}
