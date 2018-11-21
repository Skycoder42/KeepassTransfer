#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtMvvmCore/Binding>

MainWindow::MainWindow(QtMvvm::ViewModel *viewModel, QWidget *parent) :
	QMainWindow(parent),
	_viewModel(static_cast<MainViewModel*>(viewModel)),
	_ui(new Ui::MainWindow)
{
	_ui->setupUi(this);

	connect(_ui->actionSettings, &QAction::triggered,
			_viewModel, &MainViewModel::showSettings);

	QtMvvm::bind(_viewModel, "text",
				 _ui->lineEdit, "text");
	QtMvvm::bind(_viewModel, "text",
				 _ui->label, "text",
				 QtMvvm::Binding::OneWayToView);
}

MainWindow::~MainWindow()
{
	delete _ui;
}
