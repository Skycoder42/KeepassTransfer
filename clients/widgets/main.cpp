#include <QtWidgets/QApplication>
#include <QtMvvmWidgets/WidgetsPresenter>
#include <kptclientapp.h>

#include "mainwindow.h"

// Register the core app
QTMVVM_REGISTER_CORE_APP(KPTClientApp)

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	// Automatically sets "WidgetPresenter" as presenter and registers the %{WindowName} class as a widget
	// The viewmodel this widget belongs to is detected automatically via naming conventions
	QtMvvm::WidgetsPresenter::registerView<MainWindow>();

	return a.exec();
}
