#ifndef KPTSERVICE_H
#define KPTSERVICE_H

#include <QSettings>
#include <QtService/Service>

#include "transferserver.h"

class KPTService : public QtService::Service
{
	Q_OBJECT

public:
	explicit KPTService(int &argc, char **argv);

protected:
	CommandResult onStart() override;
	CommandResult onStop(int &exitCode) override;

private:
	QSettings *_settings = nullptr;
	TransferServer *_server = nullptr;
};

#undef qService
#define qService static_cast<KPTService*>(QtService::Service::instance())

#endif // KPTSERVICE_H
