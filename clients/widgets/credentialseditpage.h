#ifndef CREDENTIALSEDITDIALOG_H
#define CREDENTIALSEDITDIALOG_H

#include <QScopedPointer>
#include <QWizardPage>
#include <credentialseditviewmodel.h>

namespace Ui {
class CredentialsEditPage;
}

class CredentialsEditPage : public QWizardPage
{
	Q_OBJECT

public:
	Q_INVOKABLE explicit CredentialsEditPage(QtMvvm::ViewModel *viewModel, QWidget *parent = nullptr);
	~CredentialsEditPage() override;

	bool validatePage() override;

private:
	CredentialsEditViewModel *_viewModel;
	QScopedPointer<Ui::CredentialsEditPage> _ui;
};

#endif // CREDENTIALSEDITDIALOG_H
