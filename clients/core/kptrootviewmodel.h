#ifndef KPTROOTVIEWMODEL_H
#define KPTROOTVIEWMODEL_H

#include <QtMvvmCore/ViewModel>

class KptRootViewModel : public QtMvvm::ViewModel
{
	Q_OBJECT
	QTMVVM_SINGLETON

public:
	Q_INVOKABLE explicit KptRootViewModel(QObject *parent = nullptr);

public slots:
	void showSettings();
	void about();
};

#endif // KPTROOTVIEWMODEL_H
