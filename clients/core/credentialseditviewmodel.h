#ifndef CREDENTIALSEDITVIEWMODEL_H
#define CREDENTIALSEDITVIEWMODEL_H

#include <QtMvvmCore/ViewModel>
#include <qgadgetlistmodel.h>
#include <credential.h>

#ifdef USE_KPXCCLIENT_LIB
class KPXCClientImporter;
#endif

class CredentialsEditViewModel : public QtMvvm::ViewModel
{
	Q_OBJECT
	QTMVVM_CONTAINER_VM(KptRootViewModel)

	Q_PROPERTY(CredentialsEditViewModel::CredentialsModel* credentialsModel READ credentialsModel CONSTANT)

public:
	static const QString paramKpxcPath;
	static QVariantHash params(const QString &kpxcPath);

	using CredentialsModel = QGenericListModel<Credential>;

	Q_INVOKABLE explicit CredentialsEditViewModel(QObject *parent = nullptr);

	CredentialsModel *credentialsModel() const;

	Q_INVOKABLE bool commitCredentials();

public slots:
	void onInit(const QVariantHash &params) override;

	void addEmptyEntry();
	void importFromKPXC();

private slots:
	void entryImported(const QList<Credential> &credentials);

private:
	CredentialsModel *_credModel;
#ifdef USE_KPXCCLIENT_LIB
	KPXCClientImporter *_importer;
#endif

	void setupModel();
};

Q_DECLARE_METATYPE(CredentialsEditViewModel::CredentialsModel*)

#endif // CREDENTIALSEDITVIEWMODEL_H
