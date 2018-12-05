#include "credentialsselectionpage.h"
#include "ui_credentialsselectionpage.h"

CredentialsSelectionPage::CredentialsSelectionPage(QtMvvm::ViewModel *viewModel, QWidget *parent) :
	QWizardPage{parent},
	_viewModel{static_cast<CredentialsSelectionViewModel*>(viewModel)},
	_ui{new Ui::CredentialsSelectionPage{}}
{
	_ui->setupUi(this);
	_ui->selectButton->setDefaultAction(_ui->actionSelect_all);
	_ui->deselectButton->setDefaultAction(_ui->actionDeselect_all);
	_ui->listView->addActions({
								  _ui->actionSelect_all,
								  _ui->actionDeselect_all
							  });

	_ui->listView->setModel(_viewModel->credModel());
	connect(_ui->actionSelect_all, &QAction::triggered,
			_viewModel, &CredentialsSelectionViewModel::selectAll);
	connect(_ui->actionDeselect_all, &QAction::triggered,
			_viewModel, &CredentialsSelectionViewModel::deselectAll);
}

bool CredentialsSelectionPage::validatePage()
{
	return _viewModel->commitCredentials();
}

CredentialsSelectionPage::~CredentialsSelectionPage() = default;
