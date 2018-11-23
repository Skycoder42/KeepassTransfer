#include "kptclientapp.h"
#include <QGuiApplication>
#include <QCommandLineParser>
#include <QtMvvmCore/ServiceRegistry>
#include "credentialseditviewmodel.h"
#include "encryptionservice.h"

KPTClientApp::KPTClientApp(QObject *parent) :
	CoreApp{parent}
{
	QCoreApplication::setApplicationName(QStringLiteral(PROJECT_TARGET));
	QCoreApplication::setApplicationVersion(QStringLiteral(VERSION));
	QCoreApplication::setOrganizationName(QStringLiteral(COMPANY));
	QCoreApplication::setOrganizationDomain(QStringLiteral(BUNDLE));
	QGuiApplication::setApplicationDisplayName(QStringLiteral(PROJECT_NAME));
	//TODO QGuiApplication::setWindowIcon(QIcon{QStringLiteral(":/icons/locked.svg")});
}

void KPTClientApp::performRegistrations()
{
	//if you are using a qt resource (e.g. "clientscore.qrc"), initialize it here
	Q_INIT_RESOURCE(clientscore);
}

int KPTClientApp::startApp(const QStringList &arguments)
{
	QCommandLineParser parser;
	parser.addVersionOption();
	parser.addHelpOption();

	//add more options

	//shows help or version automatically
	if(!autoParse(parser, arguments))
		return EXIT_SUCCESS;

	// register services
	QtMvvm::ServiceRegistry::instance()->registerObject<EncryptionService>();

	//show a viewmodel to complete the startup
	show<CredentialsEditViewModel>();
	return EXIT_SUCCESS;
}
