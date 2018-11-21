#include "kptclientapp.h"
#include "credentialseditviewmodel.h"
#include <QGuiApplication>
#include <QCommandLineParser>

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

	//show a viewmodel to complete the startup
	show<CredentialsEditViewModel>();
	return EXIT_SUCCESS;
}
