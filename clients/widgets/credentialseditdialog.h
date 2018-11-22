#ifndef CREDENTIALSEDITDIALOG_H
#define CREDENTIALSEDITDIALOG_H

#include <QScopedPointer>
#include <QDialog>
#include <credentialseditviewmodel.h>

namespace Ui {
class CredentialsEditDialog;
}

class CredentialsEditDialog : public QDialog
{
	Q_OBJECT

public:
	Q_INVOKABLE explicit CredentialsEditDialog(QtMvvm::ViewModel *viewModel, QWidget *parent = nullptr);
	~CredentialsEditDialog() override;

public slots:
	void accept() override;

private:
	CredentialsEditViewModel *_viewModel;
	QScopedPointer<Ui::CredentialsEditDialog> _ui;
};

#endif // CREDENTIALSEDITDIALOG_H
