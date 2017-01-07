#include "appserver.h"

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QtBackgroundProcess>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
	QtBackgroundProcess::App a(argc, argv);
	QCoreApplication::setApplicationName(TARGET);
	QCoreApplication::setApplicationVersion(VERSION);
	QCoreApplication::setOrganizationName(COMPANY);
	QCoreApplication::setOrganizationDomain("com.skycoder42");

	a.setParserSetupFunction([](QCommandLineParser &parser){
		parser.setApplicationDescription(DISPLAY_NAME);
		parser.addPositionalArgument("port", "The port to listen on.");
	});

	a.setStartupFunction([](const QCommandLineParser &parser) {
		qApp->setForwardMasterLog(true);
		if(parser.positionalArguments().size() < 2)
			return EXIT_FAILURE;
		auto server = new AppServer(qApp);
		return server->start(parser.positionalArguments()[1].toInt()) ? EXIT_SUCCESS : EXIT_FAILURE;
	});

	return a.exec();
}

/*\
 * Communication Flow:
 * - web client sends the secret to be identified
 * - backend remembers the client under it's secret
 * - mobile client sends secret and data
 * - backend remebers the client under it's secret
 * - backend finds the web client with the same secret
 * - backend sends data to the found web client
 * - web client validates the data
 * - web client sends success/fail
 * - backend passes success/fail to the mobile client
 * - mobile client shows the information
 * - web client disconnects
 * - backend cleans up the data
 * - mobile client disconnects
 * - backend cleans up the data
\*/
