#include "credentialseditviewmodel.h"
#include <QDebug>

CredentialsEditViewModel::CredentialsEditViewModel(QObject *parent) :
	ViewModel{parent},
	_credModel{new CredentialsModel{this}}
{
	addEmptyEntry();
}

CredentialsModel *CredentialsEditViewModel::credentialsModel() const
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


CredentialsModel::CredentialsModel(QObject *parent) :
	QGenericListModel{parent}
{
	setEditable(true);
	auto column = addColumn(tr("Key"), "key");
	addRole(column, Qt::EditRole, "key");
	column = addColumn(tr("Value"), "value");
	addRole(column, Qt::EditRole, "value");
	column = addColumn(tr("Confidential"), "confidential");
	addRole(column, Qt::CheckStateRole, "confidential");
}

Qt::ItemFlags CredentialsModel::flags(const QModelIndex &index) const
{
	auto flags = QGenericListModel<Credential>::flags(index);
	if(index.column() == 2) {
		flags |= Qt::ItemIsUserCheckable;
		flags &= ~Qt::ItemIsEditable;
	}
	return flags;
}

QVariant CredentialsModel::originalData(const QModelIndex &index, int role) const
{
	//TODO move this conversion to QModelAliasBaseAdapter
	//TODO should be possible to convert per orig role OR per alias column and role
	auto resData = QGenericListModel<Credential>::originalData(index, role);
	if(roleNames().value(role) == "confidential")
		resData = QVariant::fromValue(resData.toBool() ? Qt::Checked : Qt::Unchecked);
	return resData;
}

bool CredentialsModel::setOriginalData(const QModelIndex &index, const QVariant &value, int role)
{
	if(roleNames().value(role) == "confidential")
		return QGenericListModel<Credential>::setOriginalData(index, value.value<Qt::CheckState>() != Qt::Unchecked, role);
	else
		return QGenericListModel<Credential>::setOriginalData(index, value, role);
}
