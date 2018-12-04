#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtMvvmQuick/QuickPresenter>
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif
#include <kptclientapp.h>
#include <clienttransferservice.h>
#include <kptrootviewmodel.h>
#include <credentialseditviewmodel.h>
#include <transferselectionviewmodel.h>
#include <qrcodeconnectorviewmodel.h>
#include <qrcodescanner.h>
#include <transferpreselection.h>
#include "transferloader.h"

QTMVVM_REGISTER_CORE_APP(KPTClientApp)

namespace {

void setStatusBarColor(const QColor &color)
{
#ifdef Q_OS_ANDROID
	if(QtAndroid::androidSdkVersion() >= 21) {
		auto activity = QtAndroid::androidActivity();
		QtAndroid::runOnAndroidThreadSync([=](){
			const auto FLAG_TRANSLUCENT_STATUS = QAndroidJniObject::getStaticField<jint>("android/view/WindowManager$LayoutParams",
																						 "FLAG_TRANSLUCENT_STATUS");
			const auto FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS = QAndroidJniObject::getStaticField<jint>("android/view/WindowManager$LayoutParams",
																								   "FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS");
			const auto jColor = QAndroidJniObject::callStaticMethod<jint>("android/graphics/Color",
																		  "parseColor",
																		  "(Ljava/lang/String;)I",
																		  QAndroidJniObject::fromString(color.name()).object());

			QAndroidJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");
			if(window.isValid()) {
				window.callMethod<void>("clearFlags", "(I)V", FLAG_TRANSLUCENT_STATUS);
				window.callMethod<void>("addFlags", "(I)V", FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
				window.callMethod<void>("setStatusBarColor", "(I)V", jColor);
			}
		});
	}
#else
	Q_UNUSED(color);
#endif
}

}

int main(int argc, char *argv[])
{
	setStatusBarColor(QColor(0x38, 0x8E, 0x3C));//see qtquickcontrols2.conf

	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	// If you want to support file dialogs on platforms other then android, use a QApplication instead (and add QT += widgets to the pro file)
	QGuiApplication app(argc, argv);

	coreApp->overwriteInitCredentials(TransferLoader::loadCredentials());

	QtMvvm::QuickPresenter::getInputViewFactory()->addSimpleInput<TransferPreSelection>(QStringLiteral("qrc:/qtmvvm/inputs/TransferPreSelectionEdit.qml"));
	QtMvvm::QuickPresenter::getInputViewFactory()->addInputAlias("TransferPreSelection", "QList<TransferPreSelectionEntry>");

	qmlRegisterInterface<IClientEncryptor>("IClientEncryptor");
	qmlRegisterType<QrCodeScanner>("de.skycoder42.kpt", 1, 0, "QrCodeScanner");
	qmlRegisterUncreatableType<KptRootViewModel>("de.skycoder42.kpt", 1, 0, "KptRootViewModel", QStringLiteral("ViewModels cannot be created!"));
	qmlRegisterUncreatableType<CredentialsEditViewModel>("de.skycoder42.kpt", 1, 0, "CredentialsEditViewModel", QStringLiteral("ViewModels cannot be created!"));
	qmlRegisterUncreatableType<TransferSelectionViewModel>("de.skycoder42.kpt", 1, 0, "TransferSelectionViewModel", QStringLiteral("ViewModels cannot be created!"));
	qmlRegisterUncreatableType<QrCodeConnectorViewModel>("de.skycoder42.kpt", 1, 0, "QrCodeConnectorViewModel", QStringLiteral("ViewModels cannot be created!"));

	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:/App.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
