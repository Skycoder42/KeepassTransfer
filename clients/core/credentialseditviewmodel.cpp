#include "credentialseditviewmodel.h"
#include "transferselectionviewmodel.h"
#include <QtMvvmCore/Messages>

CredentialsEditViewModel::CredentialsEditViewModel(QObject *parent) :
	ViewModel{parent},
	_credModel{new CredentialsModel{this}}
{
	setupModel();
	addEmptyEntry();
}

CredentialsEditViewModel::CredentialsModel *CredentialsEditViewModel::credentialsModel() const
{
	return _credModel;
}

bool CredentialsEditViewModel::commitCredentials()
{
	auto creds = _credModel->gadgets();
	Credential::filterOutEmpty(creds);
	if(creds.isEmpty()) {
		QtMvvm::critical(tr("Invalid Credentials"),
						 tr("You must enter at least one credential before you can continue!"));
		return false;
	} else {
		show<TransferSelectionViewModel>(TransferSelectionViewModel::params(creds));
		return true;
	}
}

void CredentialsEditViewModel::addEmptyEntry()
{
	_credModel->addGadget({});
}

void CredentialsEditViewModel::setupModel()
{
	_credModel->setEditable(true);

	// key column
	auto column = _credModel->addColumn(tr("Key"), "key");
	_credModel->addRole(column, Qt::EditRole, "key");

	// value column
	column = _credModel->addColumn(tr("Value"), "value");
	_credModel->addRole(column, Qt::EditRole, "value");

	// confidential column
	column = _credModel->addColumn(tr("Confidential"), "confidential");
	_credModel->addRole(column, Qt::CheckStateRole, "confidential");
	using Convert = CredentialsModel::Convert;
	_credModel->setExtraFlags(column, Qt::ItemIsUserCheckable, Qt::ItemIsEditable);
	_credModel->addAliasConverter(column, Qt::DisplayRole, [](Convert c, const QVariant &value){
		if(c == Convert::Read)
			return QVariant{value.toBool() ? tr("Confidential") : tr("Common")};
		else
			return QVariant{};
	});
	_credModel->addAliasConverter(column, Qt::CheckStateRole, [](Convert c, const QVariant &value){
		if(c == Convert::Read)
			return QVariant::fromValue(value.toBool() ? Qt::Checked : Qt::Unchecked);
		else if(c == Convert::Write)
			return QVariant{value.value<Qt::CheckState>() != Qt::Unchecked};
		else
			Q_UNREACHABLE();
	});
}
