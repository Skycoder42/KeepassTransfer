#ifndef QRCODECONNECTORPAGE_H
#define QRCODECONNECTORPAGE_H

#include <QScopedPointer>
#include <QWizardPage>
#include <qrcodeconnectorviewmodel.h>

namespace Ui {
class QrCodeConnectorPage;
}

class QrCodeConnectorPage : public QWizardPage
{
	Q_OBJECT

public:
	Q_INVOKABLE explicit QrCodeConnectorPage(QtMvvm::ViewModel *viewModel, QWidget *parent = nullptr);
	~QrCodeConnectorPage();

	bool validatePage() override;
	bool isComplete() const override;

private slots:
	void pasteQrData();
	void qrDataChanged();

private:
	QrCodeConnectorViewModel *_viewModel;
	QScopedPointer<Ui::QrCodeConnectorPage> _ui;
};

#endif // QRCODECONNECTORPAGE_H
