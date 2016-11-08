#include "appserver.h"
#include "clienthandler.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>

ClientHandler::ClientHandler(QWebSocket *socket, AppServer *parent) :
	QObject(parent),
	socket(socket),
	mode(Undefined),
	mySecret()
{
	connect(socket, &QWebSocket::textMessageReceived,
			this, &ClientHandler::textMessageReceived);
	connect(socket, &QWebSocket::disconnected,
			this, &ClientHandler::deleteLater);
	connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
			this, &ClientHandler::error);
	connect(socket, &QWebSocket::sslErrors,
			this, &ClientHandler::sslErrors);
}

void ClientHandler::sendData(QJsonValue data)
{
	QJsonDocument doc;
	if(data.isArray())
		doc = QJsonDocument(data.toArray());
	else if(data.isObject())
		doc = QJsonDocument(data.toObject());
	else {
		QJsonObject tObj;
		tObj["data"] = data;
		doc = QJsonDocument(tObj);
	}

	socket->sendTextMessage(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
}

void ClientHandler::textMessageReceived(const QString &message)
{
	//extract the json
	QJsonParseError error;
	auto object = QJsonDocument::fromJson(message.toUtf8(), &error).object();
	if(error.error != QJsonParseError::NoError) {
		qWarning() << "JSON Error for"
				   << socket->peerAddress().toString()
				   << "with error:"
				   << error.errorString();
		socket->close(QWebSocketProtocol::CloseCodeProtocolError);
		return;
	}

	switch (mode) {
	case ClientHandler::Undefined:
		if(object["MessageType"] == "MC") {
			mode = MobileClient;
			readInitialMobileData(object);
		} else if(object["MessageType"] == "WC") {
			mode = WebClient;
			readInitialWebData(object);
		} else {
			qWarning() << "Invalid message from"
					   << socket->peerAddress().toString()
					   << "received:"
					   << object;
			socket->close(QWebSocketProtocol::CloseCodeProtocolError);
			return;
		}
		break;
	case ClientHandler::WebClient:
		readWebStatus(object);
		break;
	case ClientHandler::MobileClient:
		//No data expected from client --> ignore
		break;
	default:
		Q_UNREACHABLE();
	}
}

void ClientHandler::error(QAbstractSocket::SocketError error)
{
	qWarning() << "Socket error for"
			   << socket->peerAddress().toString()
			   << "occured ("
			   << error
			   << "):"
			   << socket->errorString();
	socket->close(QWebSocketProtocol::CloseCodeProtocolError);
}

void ClientHandler::sslErrors(const QList<QSslError> &errors)
{
	qWarning() << errors.size() << "Socket SSL errors have occured:";
	foreach(auto error, errors) {
		qWarning() << "Socket SSL error for"
				   << socket->peerAddress().toString()
				   << "occured("
				   << error.error()
				   << "):"
				   << error.errorString();
	}
	socket->close(QWebSocketProtocol::CloseCodeProtocolError);
}

AppServer *ClientHandler::appServer()
{
	return static_cast<AppServer*>(parent());
}

void ClientHandler::readInitialMobileData(QJsonObject secretData)
{
	auto secret = secretData["Secret"].toString();
	auto data = secretData["TransferData"];
	if(secret.isEmpty() || !data.isArray()) {
		qWarning() << "No/Invalid secret or data in message from mobile client"
				   << socket->peerAddress().toString()
				   << "found:"
				   << secretData;
		socket->close(QWebSocketProtocol::CloseCodeProtocolError);
		return;
	}

	mySecret = secret;
	appServer()->registerMobileClient(secret, this);
	auto webClient = appServer()->findWebClient(secret);
	if(!webClient) {
		//try again after 5 sec
		QTimer::singleShot(5000, this, [=](){
			auto webClient = appServer()->findWebClient(secret);
			if(!webClient) {
				qWarning() << "No web client found for mobile client"
						   << socket->peerAddress().toString()
						   << "with secret:"
						   << secret;
				socket->close();
			} else
				webClient->sendData(data);
		});
	} else
		webClient->sendData(data);
}

void ClientHandler::readInitialWebData(QJsonObject secretData)
{
	auto secret = secretData["Secret"].toString();
	if(secret.isEmpty()) {
		qWarning() << "No secret in message from web client"
				   << socket->peerAddress().toString()
				   << "found:"
				   << secretData;
		socket->close(QWebSocketProtocol::CloseCodeProtocolError);
		return;
	}

	mySecret = secret;
	appServer()->registerWebClient(secret, this);
}

void ClientHandler::readWebStatus(QJsonObject status)
{
	//simply transfer to mobile client
	auto client = appServer()->findMobileClient(mySecret);
	if(client)
		client->sendData(status);
	else {
		qWarning() << "Web client for"
				   << socket->peerAddress().toString()
				   << "not found anymore with secret:"
				   << mySecret;
		socket->close();
	}
}
