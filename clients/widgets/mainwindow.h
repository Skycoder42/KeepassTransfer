#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

#include <mainviewmodel.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	Q_INVOKABLE MainWindow(QtMvvm::ViewModel *viewModel, QWidget *parent = nullptr);
	~MainWindow();

private:
	MainViewModel *_viewModel;
	Ui::MainWindow *_ui;
};

#endif // MAINWINDOW_H
