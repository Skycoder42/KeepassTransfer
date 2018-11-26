#include "clienttransferservice.h"
#include <QtConcurrent/QtConcurrentRun>
#include <QtMvvmCore/Messages>
#include <qtcoroutine.h>
#include <qtcoawaitables.h>
#include <qtcoconcurrent.h>
#include <messages/clienttransfermessage.h>
using namespace CryptoPP;

QThreadStorage<CryptoPP::AutoSeededRandomPool> ClientTransferService::_rngs;

ClientTransferService::ClientTransferService(QObject *parent) :
	QObject{parent}
{}

void ClientTransferService::sendCredentials(IClientEncryptor *clientCrypt, const QList<Credential> &credentials)
{
	Q_ASSERT(_encryptor);
	QtCoroutine::createAndRun([=](){
		auto control = QtMvvm::showBusy(this,
										tr("Transferring data"),
										tr("Encrypting and transferring credentials to the partner. Please wait…"),
										false);

		QWebSocket *socket = nullptr;
		try {
			ClientTransferMessage message;
			message.channelId = clientCrypt->channelId();
			message.data = encrypt(clientCrypt, credentials);

			//TODO obtain origin/url from browser
			const QUrl targetUrl{QStringLiteral("ws://localhost:27352")};
			socket = new QWebSocket{targetUrl.authority(), QWebSocketProtocol::VersionLatest, this};
			const auto cancelCon = connect(socket, &QWebSocket::destroyed,
										   socket, std::bind(&QtCoroutine::cancel, QtCoroutine::current()));
			connect(socket, &QWebSocket::disconnected,
					socket, &QWebSocket::deleteLater);
			connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
					this, std::bind(&ClientTransferService::onSocketError, this, socket, control));

			socket->open(targetUrl);
			QtCoroutine::await(socket, &QWebSocket::connected);
			socket->sendBinaryMessage(KPTLib::serializeMessage(message));

			KPTLib::MessageVisitor visitor;
			visitor.addFallbackVisitor(this, &ClientTransferService::onFallback, control);
			visitor.addVisitor(this, &ClientTransferService::onServerOk, control);
			visitor.addVisitor(this, &ClientTransferService::onError, control);
			const auto ok = visitor.visit(QtCoroutine::awaitargs<QByteArray>::await(socket, &QWebSocket::binaryMessageReceived));

			disconnect(cancelCon);
			socket->close(ok ? QWebSocketProtocol::CloseCodeNormal : QWebSocketProtocol::CloseCodeBadOperation);
		} catch(std::exception &e) {
			qCritical() << "Exception thrown:" << e.what();
			QMetaObject::invokeMethod(control, "close", Qt::QueuedConnection);
			QtMvvm::critical(tr("Transfer failed!"),
							 tr("An internal error occured. Unable to encrypt data."));

			if(socket) {
				if(socket->state() == QAbstractSocket::ConnectedState) {
					const auto ok = QMetaObject::invokeMethod(socket, "close", Qt::QueuedConnection);
					Q_ASSERT(ok); //TODO debug remove
				} else
					socket->deleteLater();
			}
		}
	});
}

EncryptedData ClientTransferService::encrypt(IClientEncryptor *clientCrypt, const QList<Credential> &credentials)
{
	return QtCoroutine::asyncAwait([this, clientCrypt, credentials](){
		auto &rng = _rngs.localData();

		const auto plain = KPTLib::encode(credentials);
		QByteArray iv{AES::BLOCKSIZE, Qt::Uninitialized};
		rng.GenerateBlock(reinterpret_cast<byte*>(iv.data()), AES::BLOCKSIZE);
		rng.DiscardBytes(qChecksum(plain.constData(), static_cast<uint>(plain.size())));
		auto keys = clientCrypt->obtainKeys(rng);
		auto cipher = _encryptor->encryptSymmetric(plain, keys.first, iv);

		//TODO test try/catch
		return EncryptedData {
			clientCrypt->mode(),
			std::move(keys.second),
			std::move(iv),
			std::move(cipher)
		};
	});
}

void ClientTransferService::onServerOk(const ServerOkMessage message, QtMvvm::ProgressControl *control)
{
	Q_UNUSED(message);
	control->close();
	QtMvvm::information(tr("Transfer completed!"),
						tr("Data was transferred to the client successfully"));
}

void ClientTransferService::onError(const ErrorMessage &message, QtMvvm::ProgressControl *control)
{
	control->close();
	QtMvvm::critical(tr("Transfer failed!"),
					 tr("<p>Failed to transfer credentials to partner with error:</p>"
						"<p><i>%1</i></p>")
					 .arg(message.message.toHtmlEscaped()));
}

void ClientTransferService::onFallback(int typeId, QtMvvm::ProgressControl *control)
{
	qWarning() << "Invalid message received - unexpected message type:"
			   << QMetaType::typeName(typeId);
	control->close();
	QtMvvm::critical(tr("Transfer failed!"),
					 tr("Failed to transfer credentials to partner. "
						"An invalid reply was received, the connection seems to have been cut."));
}

void ClientTransferService::onSocketError(QWebSocket *socket, QtMvvm::ProgressControl *control)
{
	qWarning() << "Socket-Error:" << socket->error();
	control->close();
	QtMvvm::critical(tr("Transfer failed!"),
					 tr("<p>Failed to transfer credentials to partner with error:</p>"
						"<p><i>%1</i></p>")
					 .arg(socket->errorString().toHtmlEscaped()));

	const auto ok = QMetaObject::invokeMethod(socket, "close", Qt::QueuedConnection,
											  Q_ARG(QWebSocketProtocol::CloseCode, QWebSocketProtocol::CloseCodeProtocolError));
	Q_ASSERT(ok); //TODO debug remove
}



IClientEncryptor::IClientEncryptor() = default;

IClientEncryptor::~IClientEncryptor() = default;
