#include "credentialseditviewmodel.h"
#include <QDebug>

CredentialsEditViewModel::CredentialsEditViewModel(QObject *parent) :
	ViewModel{parent},
	_credModel{new QGadgetListModel<Credential>{this}}
{
	addEmptyEntry();
}

QGadgetListModel<Credential> *CredentialsEditViewModel::credentialsModel() const
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

QAbstractListModel *CredentialsEditViewModel::getCredentialsModel() const
{
	return credentialsModel();
}
