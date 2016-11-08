#include "appserver.h"
#include "clienthandler.h"
#include <QDebug>
#include <QWebSocketCorsAuthenticator>

AppServer::AppServer(QObject *parent) :
	QObject(parent),
	server(new QWebSocketServer("KeepassTransferServer", QWebSocketServer::NonSecureMode, this)),
	webClients(),
	mobileClients()
{
	connect(server, &QWebSocketServer::originAuthenticationRequired,
			this, &AppServer::originAuthenticationRequired);
	connect(server, &QWebSocketServer::newConnection,
			this, &AppServer::newConnection);
	connect(server, &QWebSocketServer::acceptError,
			this, &AppServer::acceptError);
	connect(server, &QWebSocketServer::serverError,
			this, &AppServer::serverError);
	connect(server, &QWebSocketServer::sslErrors,
			this, &AppServer::sslErrors);
}

void AppServer::registerWebClient(QString secret, ClientHandler *handler)
{
	webClients.insert(secret, handler);
	connect(handler, &ClientHandler::destroyed, [=](){
		webClients.remove(secret);
	});
}

void AppServer::registerMobileClient(QString secret, ClientHandler *handler)
{
	mobileClients.insert(secret, handler);
	connect(handler, &ClientHandler::destroyed, [=](){
		mobileClients.remove(secret);
	});
}

ClientHandler *AppServer::findWebClient(QString secret) const
{
	return webClients.value(secret, nullptr);
}

ClientHandler *AppServer::findMobileClient(QString secret) const
{
	return mobileClients.value(secret, nullptr);
}

bool AppServer::start(int port)
{
	if(server->listen(QHostAddress::LocalHost, (quint16)port))
		return true;
	else {
		qCritical() << "Failed to start server on port" << port;
		return false;
	}
}

void AppServer::originAuthenticationRequired(QWebSocketCorsAuthenticator *authenticator)
{
	qDebug() << "authenication required for:" << authenticator->origin();
	authenticator->setAllowed(true);
}

void AppServer::newConnection()
{
	while(server->hasPendingConnections()) {
		qDebug() << "new connection";
		new ClientHandler(server->nextPendingConnection(), this);
	}
}

void AppServer::acceptError(QAbstractSocket::SocketError socketError)
{
	qWarning() << "Accept error occured ("
			   << socketError
			   << "):"
			   << server->errorString();
}

void AppServer::serverError(QWebSocketProtocol::CloseCode closeCode)
{
	qWarning() << "Server error occured ("
			   << closeCode
			   << "):"
			   << server->errorString();
}

void AppServer::sslErrors(const QList<QSslError> &errors)
{
	qWarning() << errors.size() << "SSL errors have occured:";
	foreach(auto error, errors) {
		qWarning() << "SSL error occured("
				   << error.error()
				   << "):"
				   << error.errorString();
	}
}
