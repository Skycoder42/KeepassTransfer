#include "credentialseditpage.h"
#include "ui_credentialseditpage.h"

CredentialsEditPage::CredentialsEditPage(QtMvvm::ViewModel *viewModel, QWidget *parent) :
	QWizardPage{parent},
	_viewModel{static_cast<CredentialsEditViewModel*>(viewModel)},
	_ui{new Ui::CredentialsEditPage{}}
{
	_ui->setupUi(this);

	_ui->addButton->setDefaultAction(_ui->action_Add_Entry);

	connect(_ui->action_Add_Entry, &QAction::triggered,
			_viewModel, &CredentialsEditViewModel::addEmptyEntry);

	_ui->tableView->addAction(_ui->action_Add_Entry);
	_ui->tableView->setModel(_viewModel->credentialsModel());
	_ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
	_ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	_ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
}

CredentialsEditPage::~CredentialsEditPage() = default;

bool CredentialsEditPage::validatePage()
{
	return _viewModel->commitCredentials();
}
