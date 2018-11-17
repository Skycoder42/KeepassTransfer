#include "kptservice.h"

#include <QThread>

KPTService::KPTService(int &argc, char **argv) :
	Service{argc, argv}
{
	QCoreApplication::setApplicationName(QStringLiteral(PROJECT_TARGET));
	QCoreApplication::setApplicationVersion(QStringLiteral(VERSION));
	QCoreApplication::setOrganizationName(QStringLiteral(COMPANY));
	QCoreApplication::setOrganizationDomain(QStringLiteral(BUNDLE));
}

QtService::Service::CommandResult KPTService::onStart()
{
	_server = new TransferServer{this};

	const auto hostName = QStringLiteral("localhost"); //TODO make configurable
	auto socket = getSocket();
	auto ok = false;
	if(socket != -1)
		ok = _server->startServer(hostName, socket);
	else
		ok = _server->startServer(hostName, true, 27352); //TODO make configurable
	if(!ok)
		return OperationFailed;

	return OperationCompleted;
}

QtService::Service::CommandResult KPTService::onStop(int &exitCode)
{
	exitCode = EXIT_SUCCESS;
	return OperationCompleted;
}
