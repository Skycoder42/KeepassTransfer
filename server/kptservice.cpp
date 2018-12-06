#include "kptservice.h"

#include <kptlib.h>

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
	KPTLib::setup();

#ifdef Q_OS_LINUX
	QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QStringLiteral("/etc"));
#endif
	_settings = new QSettings{this};
	_settings->setFallbacksEnabled(true);

	_server = new TransferServer{this};
	const auto hostName = _settings->value(QStringLiteral("server/host"), QStringLiteral("localhost")).toString();
	auto socket = getSocket();
	auto ok = false;
	if(socket != -1)
		ok = _server->startServer(hostName, socket);
	else {
		ok = _server->startServer(hostName,
								  _settings->value(QStringLiteral("server/local"), false).toBool(),
								  static_cast<quint16>(_settings->value(QStringLiteral("server/port"), 27352).toUInt()));
	}
	if(!ok)
		return OperationFailed;

	return OperationCompleted;
}

QtService::Service::CommandResult KPTService::onStop(int &exitCode)
{
	exitCode = EXIT_SUCCESS;
	return OperationCompleted;
}
