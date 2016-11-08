#include "appserver.h"

#include <QCoreApplication>
#include <QBackgroundProcess>

int main(int argc, char *argv[])
{
	QBackgroundProcess::App a(argc, argv);
	QBackgroundProcess::App::activateTerminalDebugRedirect();

	a.setStartupFunction([](QStringList arguments) {
		QBackgroundProcess::App::activateMasterDebugRedirect();
		if(arguments.size() != 1)
			return EXIT_FAILURE;
		auto server = new AppServer(qApp);
		return server->start(arguments[0].toInt()) ? EXIT_SUCCESS : EXIT_FAILURE;
	});

	return a.exec();
}

/*\
 * Communication Flow:
 * - web client sends the secret to be identified
 * + backend remebers the client under it's secret
 * + mobile client sends secret and data
 * + backend remebers the client under it's secret
 * + backend finds the web client with the same secret
 * + backend sends data to the found web client
 * - web client validates the data
 * - web client sends success/fail
 * - backend passes success/fail to the mobile client
 * - mobile client shows the information
 * - web client disconnects
 * - backend cleans up the data
 * - mobile client disconnects
 * - backend cleans up the data
\*/
