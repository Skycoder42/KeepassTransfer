#ifndef CREDENTIALSEDITVIEWMODEL_H
#define CREDENTIALSEDITVIEWMODEL_H

#include <QtMvvmCore/ViewModel>
#include <qgadgetlistmodel.h>
#include <credential.h>

class CredentialsEditViewModel : public QtMvvm::ViewModel
{
	Q_OBJECT

	Q_PROPERTY(QAbstractListModel* credentialsModel READ getCredentialsModel CONSTANT)

public:
	Q_INVOKABLE explicit CredentialsEditViewModel(QObject *parent = nullptr);

	QGadgetListModel<Credential> *credentialsModel() const;

public slots:
	void addEmptyEntry();
	void commitCredentials();

private:
	QGadgetListModel<Credential> *_credModel;

	QAbstractListModel* getCredentialsModel() const;
};

#endif // CREDENTIALSEDITVIEWMODEL_H
