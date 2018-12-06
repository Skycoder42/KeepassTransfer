#ifndef PASSCONNECTORPAGE_H
#define PASSCONNECTORPAGE_H

#include <QScopedPointer>
#include <QWizardPage>
#include <passconnectorviewmodel.h>

namespace Ui {
class PassConnectorPage;
}

class PassConnectorPage : public QWizardPage
{
	Q_OBJECT

public:
	Q_INVOKABLE explicit PassConnectorPage(QtMvvm::ViewModel *viewModel, QWidget *parent = nullptr);
	~PassConnectorPage();

	bool validatePage() override;
	bool isComplete() const override;

private slots:
	void appIdChanged();
	void toggleVisible(bool visible);

private:
	PassConnectorViewModel *_viewModel;
	QScopedPointer<Ui::PassConnectorPage> _ui;
};

#endif // PASSCONNECTORPAGE_H
