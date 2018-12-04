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

	bool readCliCredentials(const QCommandLineParser &parser);
	bool readJsonCredentials(QIODevice *device);
	bool readCsvCredentials(QIODevice *device);
	bool readPlainCredentials(QIODevice *device);
};

#undef coreApp
#define coreApp static_cast<KPTClientApp*>(QtMvvm::CoreApp::instance())

#endif // KPTCLIENTAPP_H
