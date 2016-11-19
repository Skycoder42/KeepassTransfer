#include "appserver.h"

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QBackgroundProcess>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
	QBackgroundProcess::App a(argc, argv);
	QBackgroundProcess::App::activateTerminalDebugRedirect();

	QCommandLineParser parser;
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("start|stop", "Starts or stops the application", "[start|stop]");
	parser.addPositionalArgument("port", "The port to listen on");
	parser.addOption({
						 {"l", "log"},
						 "Set the desired log <level>. Possible values are:\n"
						 " - 0: log nothing\n"
						 " - 1: critical errors only\n"
						 " - 2: 1 plus warnings\n"
						 " - 3: 2 plus information messages\n"
						 " - 4: verbose - log everything",
						 "level",
						 "3"
					 });
	parser.process(a);

	QString logStr;
	switch (parser.value("l").toInt()) {
	case 0:
		logStr.prepend("\n*.critical=false");
	case 1:
		logStr.prepend("\n*.warning=false");
	case 2:
		logStr.prepend("\n*.info=false");
	case 3:
		logStr.prepend("*.debug=false");
	default:
		break;
	}
	if(!logStr.isNull())
		QLoggingCategory::setFilterRules(logStr);

	a.setStartupFunction([&parser](QStringList) {
		QBackgroundProcess::App::activateMasterDebugRedirect();
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
