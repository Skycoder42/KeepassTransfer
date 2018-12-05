#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <serverconnector.h>
#include <dataencryptor.h>

#include "qrencoder.h"
#include "passencoder.h"
#include "qrimageprovider.h"
#include "emjsconnector.h"

#ifdef QT_OS_WASM
#include "qwasmsettings.h"
#endif

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);
	QCoreApplication::setApplicationName(QStringLiteral(PROJECT_TARGET));
	QCoreApplication::setApplicationVersion(QStringLiteral(VERSION));
	QCoreApplication::setOrganizationName(QStringLiteral(COMPANY));
	QCoreApplication::setOrganizationDomain(QStringLiteral(BUNDLE));
	QGuiApplication::setApplicationDisplayName(QStringLiteral(PROJECT_NAME));
	QGuiApplication::setWindowIcon(QIcon{QStringLiteral(":/icons/main.svg")});

#ifdef QT_OS_WASM
	QWasmSettings::registerFormat();
#endif

	KPTLib::setup();

	qmlRegisterUncreatableType<Credential>("de.skycoder42.kpt", 1, 0, "Credential", {});
	qmlRegisterUncreatableType<ServerConnector>("de.skycoder42.kpt", 1, 0, "ServerConnector", {});
	qmlRegisterUncreatableType<DataEncryptor>("de.skycoder42.kpt", 1, 0, "DataEncryptor", {});
	qmlRegisterType<QrEncoder>("de.skycoder42.kpt", 1, 0, "QrEncoder");
	qmlRegisterType<PassEncoder>("de.skycoder42.kpt", 1, 0, "PassEncoder");

	ServerConnector connector{EmJsConnector::instance()->getHostUrl()};

	QQmlApplicationEngine engine;
	engine.addImageProvider(QStringLiteral("qrcode"), new QrImageProvider{});
	engine.rootContext()->setContextProperty(QStringLiteral("connector"), &connector);
	engine.rootContext()->setContextProperty(QStringLiteral("emjscon"), EmJsConnector::instance());
	engine.rootContext()->setContextProperty(QStringLiteral("appVersion"), QCoreApplication::applicationVersion());
	engine.rootContext()->setContextProperty(QStringLiteral("qtVersion"), QStringLiteral(QT_VERSION_STR));

	engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
