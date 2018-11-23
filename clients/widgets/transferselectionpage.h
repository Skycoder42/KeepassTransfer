#ifndef TRANSFERSELECTIONPAGE_H
#define TRANSFERSELECTIONPAGE_H

#include <QWizardPage>
#include <QScopedPointer>
#include <transferselectionviewmodel.h>

namespace Ui {
class TransferSelectionPage;
}

class TransferSelectionPage : public QWizardPage
{
	Q_OBJECT

public:
	Q_INVOKABLE explicit TransferSelectionPage(QtMvvm::ViewModel *viewModel, QWidget *parent = nullptr);
	~TransferSelectionPage() override;

	bool validatePage() override;

private:
	TransferSelectionViewModel *_viewModel;
	QScopedPointer<Ui::TransferSelectionPage> _ui;
};

#endif // TRANSFERSELECTIONPAGE_H
