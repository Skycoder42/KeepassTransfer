#ifndef KPTCLIENTAPP_H
#define KPTCLIENTAPP_H

#include <QtMvvmCore/CoreApp>
#include <credential.h>

class KPTClientApp : public QtMvvm::CoreApp
{
	Q_OBJECT

public:
	explicit KPTClientApp(QObject *parent = nullptr);

	void overwriteInitCredentials(const QList<Credential> &credentials);

protected:
	void performRegistrations() override;
	int startApp(const QStringList &arguments) override;

private:
	QList<Credential> _initCredentials;
};

#undef coreApp
#define coreApp static_cast<KPTClientApp*>(QtMvvm::CoreApp::instance())

#endif // KPTCLIENTAPP_H
