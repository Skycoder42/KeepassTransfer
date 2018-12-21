#include "credentialseditpage.h"
#include "ui_credentialseditpage.h"

CredentialsEditPage::CredentialsEditPage(QtMvvm::ViewModel *viewModel, QWidget *parent) :
	QWizardPage{parent},
	_viewModel{static_cast<CredentialsEditViewModel*>(viewModel)},
	_ui{new Ui::CredentialsEditPage{}}
{
	_ui->setupUi(this);

	_ui->addButton->setDefaultAction(_ui->action_Add_Entry);
	_ui->importButton->setDefaultAction(_ui->actionKPXC_Import);

	connect(_ui->action_Add_Entry, &QAction::triggered,
			_viewModel, &CredentialsEditViewModel::addEmptyEntry);
	connect(_ui->actionKPXC_Import, &QAction::triggered,
			_viewModel, &CredentialsEditViewModel::importFromKPXC);

	_ui->tableView->addActions({
								   _ui->action_Add_Entry,
								   _ui->actionKPXC_Import
							   });
	_ui->tableView->setModel(_viewModel->credentialsModel());
	_ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
	_ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	_ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

#ifndef USE_KPXCCLIENT_LIB
	_ui->actionKPXC_Import->setEnabled(false);
	_ui->actionKPXC_Import->setVisible(false);
#endif
}

CredentialsEditPage::~CredentialsEditPage() = default;

bool CredentialsEditPage::validatePage()
{
	return _viewModel->commitCredentials();
}
