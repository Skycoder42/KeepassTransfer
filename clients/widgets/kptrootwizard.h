#ifndef KPTROOTWIZARD_H
#define KPTROOTWIZARD_H

#include <QtCore/QStack>
#include <QtMvvmCore/ViewModel>
#include <QtMvvmWidgets/IPresentingView>
#include <QtWidgets/QWizard>
#include <kptrootviewmodel.h>

class KptRootWizard : public QWizard, public QtMvvm::IPresentingView
{
	Q_OBJECT
	Q_INTERFACES(QtMvvm::IPresentingView)

public:
	Q_INVOKABLE explicit KptRootWizard(QtMvvm::ViewModel *viewModel, QWidget *parent = nullptr);

	bool tryPresent(QWidget *view) override;

	bool validateCurrentPage() override;
	int nextId() const override;

protected:
	void initializePage(int id) override;
	void cleanupPage(int id) override;

private slots:
	void dropPage(int id);

private:
	KptRootViewModel *_viewModel;

	int _pageCounter = 0;
	QStack<int> _pageStack;

	mutable bool _recurseBlocker = false;

	int topStack() const;
	int nextStackId() const;
};

#endif // KPTROOTWIZARD_H
