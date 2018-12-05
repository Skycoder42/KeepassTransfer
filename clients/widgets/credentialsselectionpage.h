#ifndef CREDENTIALSSELECTIONPAGE_H
#define CREDENTIALSSELECTIONPAGE_H

#include <QScopedPointer>
#include <QWizardPage>
#include <credentialsselectionviewmodel.h>

namespace Ui {
class CredentialsSelectionPage;
}

class CredentialsSelectionPage : public QWizardPage
{
	Q_OBJECT

public:
	Q_INVOKABLE explicit CredentialsSelectionPage(QtMvvm::ViewModel *viewModel, QWidget *parent = nullptr);
	~CredentialsSelectionPage() override;

	bool validatePage() override;

private:
	CredentialsSelectionViewModel *_viewModel;
	QScopedPointer<Ui::CredentialsSelectionPage> _ui;
};

#endif // CREDENTIALSSELECTIONPAGE_H
