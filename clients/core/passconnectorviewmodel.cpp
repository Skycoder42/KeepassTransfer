#include "passconnectorviewmodel.h"
#include "transferselectionviewmodel.h"

PassConnectorViewModel::PassConnectorViewModel(QObject *parent) :
	ViewModel{parent},
	_channelIdValidator{new QRegularExpressionValidator{this}}
{
	_channelIdValidator->setRegularExpression(QRegularExpression{
												  QStringLiteral(R"__(^(?:[a-zA-Z0-9+\/]{4}-){5}[a-zA-Z0-9+\/]{4}$)__"),
												  QRegularExpression::OptimizeOnFirstUsageOption |
												  QRegularExpression::DontCaptureOption
											  });
}

QRegularExpressionValidator *PassConnectorViewModel::channelIdValidator() const
{
	return _channelIdValidator;
}

QUuid PassConnectorViewModel::channelId() const
{
	return _passCryptor ? _passCryptor->channelId() : QUuid{};
}

QString PassConnectorViewModel::channelIdStr() const
{
	return KPTLib::uiEncodeId(channelId());
}

QString PassConnectorViewModel::passphrase() const
{
	return _passCryptor ? _passCryptor->passphrase() : QString{};
}

bool PassConnectorViewModel::isValid() const
{
	return _passCryptor && !_passCryptor->channelId().isNull();
}

bool PassConnectorViewModel::transfer()
{
	if(!isValid())
		return false;

	_transferService->sendCredentials(_passCryptor, _credentials);
	return true;
}

void PassConnectorViewModel::generateRandomPassphrase()
{
	QByteArray rngData{12, Qt::Uninitialized};
	_rng.GenerateBlock(reinterpret_cast<CryptoPP::byte*>(rngData.data()),
					   static_cast<size_t>(rngData.size()));
	setPassphrase(QString::fromUtf8(rngData.toBase64(QByteArray::OmitTrailingEquals)));
}

void PassConnectorViewModel::onInit(const QVariantHash &params)
{
	Q_ASSERT(_transferService);
	Q_ASSERT(_encryptor);
	Q_ASSERT(_settings);
	connect(_transferService, &ClientTransferService::transferCompleted,
			this, &PassConnectorViewModel::transferDone);

	_passCryptor = new PassClientEncryptor{_encryptor, this};
	connect(_passCryptor, &PassClientEncryptor::channelIdChanged,
			this, &PassConnectorViewModel::channelIdChanged);
	connect(_passCryptor, &PassClientEncryptor::passphraseChanged,
			this, &PassConnectorViewModel::passphraseChanged);

	_credentials = params.value(TransferSelectionViewModel::paramCred).value<QList<Credential>>();
}

void PassConnectorViewModel::setChannelId(QUuid channelId)
{
	if(_passCryptor)
		_passCryptor->setChannelId(channelId);
}

void PassConnectorViewModel::setChannelIdStr(QString channelIdStr)
{
	setChannelId(KPTLib::uiDecodeId(std::move(channelIdStr)));
}

void PassConnectorViewModel::setPassphrase(QString passphrase)
{
	if(_passCryptor)
		_passCryptor->setPassphrase(std::move(passphrase));
}

void PassConnectorViewModel::transferDone()
{
	if(_settings->transfer.exit)
		QCoreApplication::quit();
}
