#ifndef TRANSFERSELECTIONVIEWMODEL_H
#define TRANSFERSELECTIONVIEWMODEL_H

#include <QtMvvmCore/ViewModel>
#include <QtGui/QStandardItemModel>

#include <credential.h>

class TransferSelectionViewModel : public QtMvvm::ViewModel
{
	Q_OBJECT
	QTMVVM_CONTAINER_VM(KptRootViewModel)

	Q_PROPERTY(QStandardItemModel* modeModel READ modeModel CONSTANT)

public:
	static const QString paramCred;
	static QVariantHash params(const QList<Credential> &creds);

	Q_INVOKABLE explicit TransferSelectionViewModel(QObject *parent = nullptr);

	QStandardItemModel* modeModel() const;

	Q_INVOKABLE bool startTransfer(int index);
	bool startTransfer(const QModelIndex &index);

public slots:
	void onInit(const QVariantHash &params) override;

private:
	QStandardItemModel *_modeModel;
	QList<Credential> _credentials;
};

#endif // TRANSFERSELECTIONVIEWMODEL_H
