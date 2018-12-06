#include <QtWidgets/QApplication>
#include <QtMvvmWidgets/WidgetsPresenter>
#include <kptclientapp.h>

#include "kptrootwizard.h"
#include "credentialseditpage.h"
#include "credentialsselectionpage.h"
#include "transferselectionpage.h"
#include "qrcodeconnectorpage.h"
#include "passconnectorpage.h"
#include "transferpreselectionedit.h"

// Register the core app
QTMVVM_REGISTER_CORE_APP(KPTClientApp)

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QtMvvm::WidgetsPresenter::getInputWidgetFactory()->addSimpleWidget("TransferPreSelection", [](QWidget *parent){
		return new TransferPreSelectionEdit{parent};
	});

	QtMvvm::WidgetsPresenter::registerView<KptRootWizard>();
	QtMvvm::WidgetsPresenter::registerView<CredentialsEditPage>();
	QtMvvm::WidgetsPresenter::registerView<CredentialsSelectionPage>();
	QtMvvm::WidgetsPresenter::registerView<TransferSelectionPage>();
	QtMvvm::WidgetsPresenter::registerView<QrCodeConnectorPage>();
	QtMvvm::WidgetsPresenter::registerView<PassConnectorPage>();

	return a.exec();
}
