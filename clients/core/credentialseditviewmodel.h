#ifndef CREDENTIALSEDITVIEWMODEL_H
#define CREDENTIALSEDITVIEWMODEL_H

#include <QtMvvmCore/ViewModel>
#include <qgadgetlistmodel.h>
#include <credential.h>

class CredentialsEditViewModel : public QtMvvm::ViewModel
{
	Q_OBJECT
	QTMVVM_CONTAINER_VM(KptRootViewModel)

	Q_PROPERTY(CredentialsEditViewModel::CredentialsModel* credentialsModel READ credentialsModel CONSTANT)

public:
	using CredentialsModel = QGenericListModel<Credential>;

	Q_INVOKABLE explicit CredentialsEditViewModel(QObject *parent = nullptr);

	CredentialsModel *credentialsModel() const;

	Q_INVOKABLE bool commitCredentials();

public slots:
	void addEmptyEntry();

private:
	CredentialsModel *_credModel;

	void setupModel();
};

Q_DECLARE_METATYPE(CredentialsEditViewModel::CredentialsModel*)

#endif // CREDENTIALSEDITVIEWMODEL_H
