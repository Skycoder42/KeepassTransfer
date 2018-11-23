#include "credentialseditviewmodel.h"
#include <QDebug>

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

void CredentialsEditViewModel::addEmptyEntry()
{
	_credModel->addGadget({});
}

void CredentialsEditViewModel::commitCredentials()
{
	Q_UNIMPLEMENTED();
	for(const auto &cred : _credModel->gadgets())
		qDebug() << cred.key() << cred.value() << cred.confidential();
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
