#include <QtWidgets/QApplication>
#include <QtMvvmWidgets/WidgetsPresenter>
#include <kptclientapp.h>

#include "kptrootwizard.h"
#include "credentialseditpage.h"
#include "transferselectionpage.h"
#include "qrcodeconnectorpage.h"
#include "transferpreselectionedit.h"

// Register the core app
QTMVVM_REGISTER_CORE_APP(KPTClientApp)

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QtMvvm::WidgetsPresenter::getInputWidgetFactory()->addSimpleWidget<TransferPreSelection, TransferPreSelectionEdit>();
	QtMvvm::WidgetsPresenter::getInputWidgetFactory()->addAlias("TransferPreSelection", "QList<TransferPreSelectionEntry>");

	QtMvvm::WidgetsPresenter::registerView<KptRootWizard>();
	QtMvvm::WidgetsPresenter::registerView<CredentialsEditPage>();
	QtMvvm::WidgetsPresenter::registerView<TransferSelectionPage>();
	QtMvvm::WidgetsPresenter::registerView<QrCodeConnectorPage>();

	return a.exec();
}
