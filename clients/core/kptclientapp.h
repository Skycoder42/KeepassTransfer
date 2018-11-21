#ifndef KPTCLIENTAPP_H
#define KPTCLIENTAPP_H

#include <QtMvvmCore/CoreApp>

class KPTClientApp : public QtMvvm::CoreApp
{
	Q_OBJECT

public:
	explicit KPTClientApp(QObject *parent = nullptr);

protected:
	void performRegistrations() override;
	int startApp(const QStringList &arguments) override;
};

#undef coreApp
#define coreApp static_cast<KPTClientApp*>(CoreApp::instance())

#endif // KPTCLIENTAPP_H
