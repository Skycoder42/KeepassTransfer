#ifndef CREDENTIALSEDITVIEWMODEL_H
#define CREDENTIALSEDITVIEWMODEL_H

#include <QtMvvmCore/ViewModel>
#include <qgadgetlistmodel.h>
#include <credential.h>

class CredentialsModel : public QGenericListModel<Credential>
{
	Q_OBJECT

public:
	CredentialsModel(QObject *parent = nullptr);

	Qt::ItemFlags flags(const QModelIndex& index) const override;

protected:
	QVariant originalData(const QModelIndex &index, int role) const override;
	bool setOriginalData(const QModelIndex &index, const QVariant &value, int role) override;
};

class CredentialsEditViewModel : public QtMvvm::ViewModel
{
	Q_OBJECT

	Q_PROPERTY(CredentialsModel* credentialsModel READ credentialsModel CONSTANT)

public:
	Q_INVOKABLE explicit CredentialsEditViewModel(QObject *parent = nullptr);

	CredentialsModel *credentialsModel() const;

public slots:
	void addEmptyEntry();
	void commitCredentials();

private:
	CredentialsModel *_credModel;
};

#endif // CREDENTIALSEDITVIEWMODEL_H
