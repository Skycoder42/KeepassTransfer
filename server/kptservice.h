#ifndef KPTSERVICE_H
#define KPTSERVICE_H

#include <QtService/Service>
#include <QWebSocket>
#include <QWebSocketServer>

class KPTService : public QtService::Service
{
	Q_OBJECT

public:
	explicit KPTService(int &argc, char **argv);

protected:
	CommandResult onStart() override;
	CommandResult onStop(int &exitCode) override;

private:
	QWebSocketServer *_server;
};

#undef qService
#define qService static_cast<KPTService*>(QtService::Service::instance())

#endif // KPTSERVICE_H
