#include "kptclientapp.h"
#include <QGuiApplication>
#include <QIcon>
#include <QCommandLineParser>
#include <QtMvvmCore/ServiceRegistry>
#include <QJsonSerializer>
#include "kptrootviewmodel.h"
#include "clienttransferservice.h"
#include "transferselectionviewmodel.h"
#include "transferpreselection.h"

KPTClientApp::KPTClientApp(QObject *parent) :
	CoreApp{parent}
{
	QCoreApplication::setApplicationName(QStringLiteral(PROJECT_TARGET));
	QCoreApplication::setApplicationVersion(QStringLiteral(VERSION));
	QCoreApplication::setOrganizationName(QStringLiteral(COMPANY));
	QCoreApplication::setOrganizationDomain(QStringLiteral(BUNDLE));
	QGuiApplication::setApplicationDisplayName(QStringLiteral(PROJECT_NAME));
}

void KPTClientApp::overwriteInitCredentials(const QList<Credential> &credentials)
{
	_initCredentials = credentials;
}

void KPTClientApp::performRegistrations()
{
	//if you are using a qt resource (e.g. "clientscore.qrc"), initialize it here
	Q_INIT_RESOURCE(clientscore);
	QGuiApplication::setWindowIcon(QIcon{QStringLiteral(":/icons/main.svg")});

	KPTLib::setup();

	qRegisterMetaType<TransferPreSelection>("TransferPreSelection");
	qRegisterMetaType<KptRootViewModel*>();
	QJsonSerializer::registerListConverters<TransferPreSelectionEntry>();
	registerInputTypeMapping<QVariantList>("TransferPreSelection");

	// register services
	QtMvvm::ServiceRegistry::instance()->registerObject<DataEncryptor>();
	QtMvvm::ServiceRegistry::instance()->registerObject<ClientTransferService>();
}

int KPTClientApp::startApp(const QStringList &arguments)
{
	QCommandLineParser parser;
	parser.addVersionOption();
	parser.addHelpOption();

	//TODO add more options

	//shows help or version automatically
	if(!autoParse(parser, arguments))
		return EXIT_SUCCESS;

	// invoke to prepare defaults
	Settings::instance()->preselectedSetupDummy.get();

	//show a viewmodel to complete the startup
	show<KptRootViewModel>(_initCredentials.isEmpty() ?
							   QVariantHash{} :
							   TransferSelectionViewModel::params(_initCredentials));
	return EXIT_SUCCESS;
}
