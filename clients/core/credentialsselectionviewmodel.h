#ifndef CREDENTIALSSELECTIONVIEWMODEL_H
#define CREDENTIALSSELECTIONVIEWMODEL_H

#include <QStandardItemModel>
#include <QtMvvmCore/ViewModel>
#include <credential.h>
#include "settings.h"

class CredentialsSelectionViewModel : public QtMvvm::ViewModel
{
	Q_OBJECT
	QTMVVM_CONTAINER_VM(KptRootViewModel)

	QTMVVM_INJECT_PROP(Settings*, settings, _settings)

	Q_PROPERTY(QStandardItemModel* credModel READ credModel CONSTANT)

public:
	enum Roles {
		KeyRole = Qt::DisplayRole,
		IconRole = Qt::DecorationRole,
		SelectedRole = Qt::CheckStateRole,
		IconNameRole = Qt::UserRole + 1,
		IconPathRole
	};
	Q_ENUM(Roles)

	static const QString paramCred;
	static QVariantHash params(const QList<Credential> &creds);

	Q_INVOKABLE explicit CredentialsSelectionViewModel(QObject *parent = nullptr);

	QStandardItemModel* credModel() const;

	Q_INVOKABLE bool commitCredentials();

public slots:
	void onInit(const QVariantHash &params) override;

	void selectAll();
	void deselectAll();

private:
	Settings *_settings = nullptr;
	QList<Credential> _credentials;

	QStandardItemModel * const _credModel;
	QHash<QString, QStandardItem*> _modelMap;
};

#endif // CREDENTIALSSELECTIONVIEWMODEL_H
