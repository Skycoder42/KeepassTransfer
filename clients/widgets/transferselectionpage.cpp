#include "transferselectionpage.h"
#include "ui_transferselectionpage.h"

TransferSelectionPage::TransferSelectionPage(QtMvvm::ViewModel *viewModel, QWidget *parent) :
	QWizardPage{parent},
	_viewModel{static_cast<TransferSelectionViewModel*>(viewModel)},
	_ui{new Ui::TransferSelectionPage{}}
{
	_ui->setupUi(this);
	setFinalPage(true);

	_ui->listView->setModel(_viewModel->modeModel());
}

bool TransferSelectionPage::validatePage()
{
	return _viewModel->startTransfer(_ui->listView->currentIndex());
}

TransferSelectionPage::~TransferSelectionPage() = default;
