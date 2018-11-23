#include "credentialseditdialog.h"
#include "ui_credentialseditdialog.h"

CredentialsEditDialog::CredentialsEditDialog(QtMvvm::ViewModel *viewModel, QWidget *parent) :
	QDialog{parent},
	_viewModel{static_cast<CredentialsEditViewModel*>(viewModel)},
	_ui{new Ui::CredentialsEditDialog{}}
{
	_ui->setupUi(this);

	_ui->addButton->setDefaultAction(_ui->action_Add_Entry);

	connect(_ui->action_Add_Entry, &QAction::triggered,
			_viewModel, &CredentialsEditViewModel::addEmptyEntry);

	_ui->tableView->setModel(_viewModel->credentialsModel());
	_ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
	_ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	_ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
}

void CredentialsEditDialog::accept()
{
	_viewModel->commitCredentials();
	QDialog::accept();
}

CredentialsEditDialog::~CredentialsEditDialog() = default;
