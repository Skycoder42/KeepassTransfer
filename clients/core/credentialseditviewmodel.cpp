#include "credentialseditviewmodel.h"
#include <QDebug>

CredentialsEditViewModel::CredentialsEditViewModel(QObject *parent) :
	ViewModel{parent},
	_credModel{new QGenericListModel<Credential>{this}}
{
	_credModel->setEditable(true);
	auto column = _credModel->addColumn(tr("Key"), "key");
	_credModel->addRole(column, Qt::EditRole, "key");
	column = _credModel->addColumn(tr("Value"), "value");
	_credModel->addRole(column, Qt::EditRole, "value");
	column = _credModel->addColumn(tr("Confidential"));
	_credModel->addRole(column, Qt::CheckStateRole, "value");
	//TODO add extra flags for checkable to work

	addEmptyEntry();
}

QGenericListModel<Credential> *CredentialsEditViewModel::credentialsModel() const
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

QMetaObjectModel *CredentialsEditViewModel::getCredentialsModel() const
{
	return credentialsModel();
}
