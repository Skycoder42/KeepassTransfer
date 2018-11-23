#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtMvvmQuick/QuickPresenter>
#include <kptclientapp.h>
#include <encryptionservice.h>
#include <kptrootviewmodel.h>
#include <credentialseditviewmodel.h>
#include <transferselectionviewmodel.h>

QTMVVM_REGISTER_CORE_APP(KPTClientApp)

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	// If you want to support file dialogs on platforms other then android, use a QApplication instead (and add QT += widgets to the pro file)
	QGuiApplication app(argc, argv);

	QtMvvm::QuickPresenter::getInputViewFactory(); //Workaround for QTBUG-69963
	qmlRegisterInterface<IClientEncryptor>("IClientEncryptor");
	qmlRegisterUncreatableType<KptRootViewModel>("de.skycoder42.kpt", 1, 0, "KptRootViewModel", QStringLiteral("ViewModels cannot be created!"));
	qmlRegisterUncreatableType<CredentialsEditViewModel>("de.skycoder42.kpt", 1, 0, "CredentialsEditViewModel", QStringLiteral("ViewModels cannot be created!"));
	qmlRegisterUncreatableType<TransferSelectionViewModel>("de.skycoder42.kpt", 1, 0, "TransferSelectionViewModel", QStringLiteral("ViewModels cannot be created!"));

	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:/App.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
