#include "clienttransferservice.h"
#include <QtConcurrent/QtConcurrentRun>
#include <QtMvvmCore/Messages>
#include <messages/clienttransfermessage.h>
using namespace CryptoPP;

QThreadStorage<CryptoPP::AutoSeededRandomPool> ClientTransferService::_rngs;

ClientTransferService::ClientTransferService(QObject *parent) :
	QObject{parent}
{}

void ClientTransferService::sendCredentials(IClientEncryptor *clientCrypt, const QList<Credential> &credentials)
{
	Q_ASSERT(_encryptor);
	if(_currentSocket) {
		QtMvvm::warning(tr("Already transferring data"),
						tr("You can only transfer one dataset a time. "
						   "Please wait for the first one to finish before starting another one."));
		return;
	}

	Q_ASSERT(!_currentControl);
	Q_ASSERT(!_cryptWatcher);
	Q_ASSERT(!_currentCrypt);

	_currentCrypt = clientCrypt;
	_doSend = true;

	// step 1: show the progress dialog
	_currentControl = QtMvvm::showBusy(this,
									   tr("Transferring data"),
									   tr("Encrypting and transferring credentials to the partner. Please wait…"),
									   false);

	// step 2: initiate the connection
	const QUrl targetUrl{QStringLiteral("ws://192.168.179.37:27352")}; //TODO get from settings
	_currentSocket = new QWebSocket{targetUrl.authority(), QWebSocketProtocol::VersionLatest, this};
	connect(_currentSocket, &QWebSocket::connected,
			this, &ClientTransferService::connected);
	connect(_currentSocket, &QWebSocket::disconnected,
			this, &ClientTransferService::disconnected,
			Qt::QueuedConnection);
	connect(_currentSocket, &QWebSocket::binaryMessageReceived,
			this, &ClientTransferService::binaryMessageReceived);
	connect(_currentSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
			this, &ClientTransferService::onSocketError,
			Qt::DirectConnection);
	_currentSocket->open(targetUrl);

	// step 3: start the message encryption
	_cryptWatcher = new QFutureWatcher<EncryptedData>{this};
	connect(_cryptWatcher, &QFutureWatcherBase::finished,
			this, &ClientTransferService::cryptDataReady);
	_cryptWatcher->setFuture(QtConcurrent::run(this, &ClientTransferService::encrypt, clientCrypt, credentials));
}

void ClientTransferService::connected()
{
	// step 4: send the data (if already encrypted)
	if(_doSend && _cryptWatcher->isFinished())
		sendData();
}

void ClientTransferService::disconnected()
{
	if(_currentControl) {
		_currentControl->close();
		_currentControl.clear();
	}

	if(!_cryptWatcher->isFinished())
		_cryptWatcher->cancel();
	_cryptWatcher->deleteLater();
	_cryptWatcher = nullptr;
	_currentCrypt = nullptr;
	_currentSocket->deleteLater();
	_currentSocket = nullptr;
}

void ClientTransferService::binaryMessageReceived(const QByteArray &message)
{
	if(_doSend) {
		qWarning() << "Received data before sending it - ignoring unexpected message:" << message;
		return;
	}

	// step 5: handle the reply
	KPTLib::MessageVisitor visitor;
	visitor.addFallbackVisitor(this, &ClientTransferService::onFallback);
	visitor.addVisitor(this, &ClientTransferService::onServerOk);
	visitor.addVisitor(this, &ClientTransferService::onError);
	visitor.visit(message);
}

void ClientTransferService::onSocketError()
{
	_doSend = false;
	if(_currentSocket)
		qWarning() << "Socket-Error:" << _currentSocket->error();
	_currentControl->close();
	QtMvvm::critical(tr("Transfer failed!"),
					 tr("<p>Failed to transfer credentials to partner with error:</p>"
						"<p><i>%1</i></p>")
					 .arg(_currentSocket ?
							  _currentSocket->errorString().toHtmlEscaped() :
							  tr("Unknown Error")));
}

void ClientTransferService::cryptDataReady()
{
	// step 4: send the data (if already connected)
	if(_doSend && _currentSocket->state() == QAbstractSocket::ConnectedState)
		sendData();
}

EncryptedData ClientTransferService::encrypt(IClientEncryptor *clientCrypt, const QList<Credential> &credentials)
{
	auto &rng = _rngs.localData();

	const auto plain = KPTLib::encode(credentials);
	QByteArray iv{AES::BLOCKSIZE, Qt::Uninitialized};
	rng.GenerateBlock(reinterpret_cast<byte*>(iv.data()), AES::BLOCKSIZE);
	rng.DiscardBytes(qChecksum(plain.constData(), static_cast<uint>(plain.size())));
	auto keys = clientCrypt->obtainKeys(rng);
	auto cipher = _encryptor->encryptSymmetric(plain, keys.first, iv);

	return EncryptedData {
		clientCrypt->mode(),
		std::move(keys.second),
		std::move(iv),
		std::move(cipher)
	};
}

void ClientTransferService::onServerOk(const ServerOkMessage message)
{
	Q_UNUSED(message);
	_currentControl->close();
	_currentControl.clear();
	QtMvvm::information(tr("Transfer completed!"),
						tr("Data was transferred to the client successfully"));
	_currentSocket->close();
}

void ClientTransferService::onError(const ErrorMessage &message)
{
	_currentControl->close();
	_currentControl.clear();
	QtMvvm::critical(tr("Transfer failed!"),
					 tr("<p>Failed to transfer credentials to partner with error:</p>"
						"<p><i>%1</i></p>")
					 .arg(message.message.toHtmlEscaped()));
	_currentSocket->close();
}

void ClientTransferService::onFallback(int typeId)
{
	qWarning() << "Invalid message received - unexpected message type:"
			   << QMetaType::typeName(typeId);
	_currentControl->close();
	_currentControl.clear();
	QtMvvm::critical(tr("Transfer failed!"),
					 tr("Failed to transfer credentials to partner. "
						"An invalid reply was received, the connection seems to have been cut."));
	_currentSocket->close(QWebSocketProtocol::CloseCodeBadOperation);
}

void ClientTransferService::sendData()
{
	try {
		_doSend = false;
		ClientTransferMessage message;
		message.channelId = _currentCrypt->channelId();
		message.data = _cryptWatcher->result();
		_currentSocket->sendBinaryMessage(KPTLib::serializeMessage(message));
	} catch(std::exception &e) {
		qCritical() << "Exception thrown:" << e.what();
		_currentControl->close();
		_currentControl.clear();
		QtMvvm::critical(tr("Transfer failed!"),
						 tr("An internal error occured. Unable to encrypt data."));
		_currentSocket->close();
	}
}



IClientEncryptor::IClientEncryptor() = default;

IClientEncryptor::~IClientEncryptor() = default;
