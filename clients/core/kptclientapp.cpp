#include "kptclientapp.h"
#include <QCommandLineParser>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <QGuiApplication>
#include <QIcon>

#include <QtMvvmCore/ServiceRegistry>

#include <qtcsv/reader.h>

#ifdef USE_KPXCCLIENT_LIB
#include <kpxcclient/kpxcclient_global.h>
#endif

#include "kptrootviewmodel.h"
#include "clienttransferservice.h"
#include "credentialseditviewmodel.h"
#include "credentialsselectionviewmodel.h"
#include "transferpreselectionentry.h"

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

#ifdef USE_KPXCCLIENT_LIB
	KPXCClient::init();
#endif
	KPTLib::setup();

	qRegisterMetaType<KptRootViewModel*>();
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

	parser.addOption({
						 {QStringLiteral("t"), QStringLiteral("transfer")},
						 tr("Pass password entries to the application to transfer them. "
							"You can either pass a path as last argument or let the application "
							"read the data from stdin.")
					 });
	parser.addOption({
						{QStringLiteral("f"), QStringLiteral("format")},
						 tr("Specify the <format> of that the data beeing passed has. "
							"Only works in conjunction with '--transfer'. Supported formats are:\n"
							"- json\n"
							"- csv (with ';' as seperator)\n"
							"- plain\n"
							"The default format is determined from the file extension and falls back to 'plain' for stdin."),
						 tr("format"),
						 QStringLiteral("auto")
					 });
	parser.addPositionalArgument(QStringLiteral("transfer-file-path"),
								 tr("The path to a file that contains the credentials to be transferred. "
									"Can only be specified if '--transfer' is set. If left empty, data is "
									"read from stdin (Use CTRL+D to commit data from the terminal)."),
								 QStringLiteral("[transfer-file-path]"));

	//shows help or version automatically
	if(!autoParse(parser, arguments))
		return EXIT_SUCCESS;

	// invoke to prepare defaults
	Settings::instance()->preselectedSetupDummy.get();

	// read credentials from CLI if specified
	if(parser.isSet(QStringLiteral("transfer"))) {
		if(!readCliCredentials(parser))
			return EXIT_FAILURE;
	}

	//show a viewmodel to complete the startup
	if(_initCredentials.isEmpty())
		show<CredentialsEditViewModel>();
	else
		show<CredentialsSelectionViewModel>(CredentialsSelectionViewModel::params(_initCredentials));
	return EXIT_SUCCESS;
}

bool KPTClientApp::readCliCredentials(const QCommandLineParser &parser)
{
	// step 1: open the input device
	QIODevice *inDevice = nullptr;
	QString suffix;
	if(parser.positionalArguments().size() > 0) {
		inDevice = new QFile{parser.positionalArguments().value(0), this};
		suffix = QFileInfo{parser.positionalArguments().value(0)}.suffix();
		if(!inDevice->open(QIODevice::ReadOnly | QIODevice::Text)) {
			qCritical() << "Failed to read file" << parser.positionalArguments().value(0)
						<< "with error:" << qUtf8Printable(inDevice->errorString());
			inDevice->deleteLater();
			return false;
		}
	} else {
		const auto cFile = new QFile{this};
		inDevice = cFile;
		if(!cFile->open(stdin, QIODevice::ReadOnly | QIODevice::Text)) {
			qCritical().noquote() << "Failed to read from stdin with error:"
								  << inDevice->errorString();
			inDevice->deleteLater();
			return false;
		}
	}

	Q_ASSERT(inDevice);

	// step 2: determine the format
	auto formatStr = parser.value(QStringLiteral("format"));
	if(formatStr == QStringLiteral("auto")) {
		if (suffix == QStringLiteral("json") || suffix == QStringLiteral("csv"))
			formatStr = suffix;
		else
			formatStr = QStringLiteral("plain");
	}

	bool ok;
	if(formatStr == QStringLiteral("json"))
		ok = readJsonCredentials(inDevice);
	else if(formatStr == QStringLiteral("csv"))
		ok = readCsvCredentials(inDevice);
	else if(formatStr == QStringLiteral("plain"))
		ok = readPlainCredentials(inDevice);
	else {
		qCritical() << "Unknown input format:" << formatStr;
		ok = false;
	}
	inDevice->close();
	inDevice->deleteLater();
	return ok;
}

bool KPTClientApp::readJsonCredentials(QIODevice *device)
{
	QJsonParseError error;
	const auto doc = QJsonDocument::fromJson(device->readAll(), &error);
	if(error.error != QJsonParseError::NoError) {
		qCritical().noquote() << "Failed to read json input with error:"
							  << error.errorString();
		return false;
	}

	QJsonArray credentials;
	if(doc.isArray())
		credentials = doc.array();
	else if(doc.isObject())
		credentials.append(doc.object());

	for(const auto element : credentials) {
		const auto obj = element.toObject();
		_initCredentials.append({
									obj[QStringLiteral("key")].toString(),
									obj[QStringLiteral("value")].toString(),
									obj[QStringLiteral("confidential")].toBool()
								});
	}
	return true;
}

bool KPTClientApp::readCsvCredentials(QIODevice *device)
{
	const auto data = QtCSV::Reader::readToList(*device, QStringLiteral(";"));
	auto cLine = -1;
	for(const auto &row : data) {
		++cLine;
		if(row.size() < 1 || row.size() > 3) {
			qCritical() << "Error in line" << cLine
						<< ": Expected 1-3 columns, but found"
						<< row.size();
			return false;
		}
		Credential cred{row[0]};
		if(row.size() >= 2)
			cred.setValue(row[1]);
		if(row.size() >= 3)
			cred.setConfidential(QVariant{row[2]}.toBool());
		_initCredentials.append(cred);
	}
	return true;
}

bool KPTClientApp::readPlainCredentials(QIODevice *device)
{
	auto cLine = -1;
	while(!device->atEnd()) {
		++cLine;
		const auto lineData = device->readLine().trimmed();
		if(lineData.isEmpty())
			continue;
		const auto line = lineData.split(':');
		if(line.size() < 2) {
			qCritical() << "Error in line" << cLine
						<< ": Expected 2 columns, but found"
						<< line.size();
			return false;
		}

		Credential cred;
		const auto keyPart = QString::fromUtf8(line[0]).trimmed();
		if(keyPart.endsWith(QStringLiteral("*"))) {
			cred.setKey(keyPart.mid(0, keyPart.size() - 1));
			cred.setConfidential(true);
		} else
			cred.setKey(keyPart);
		cred.setValue(QString::fromUtf8(line.mid(1).join(':')).trimmed());
		_initCredentials.append(cred);
	}
	return true;
}
