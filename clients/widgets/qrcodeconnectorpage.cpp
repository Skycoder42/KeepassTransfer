#include "qrcodeconnectorpage.h"
#include "ui_qrcodeconnectorpage.h"

QrCodeConnectorPage::QrCodeConnectorPage(QtMvvm::ViewModel *viewModel, QWidget *parent) :
	QWizardPage{parent},
	_viewModel{static_cast<QrCodeConnectorViewModel*>(viewModel)},
	_ui{new Ui::QrCodeConnectorPage{}}
{
	_ui->setupUi(this);
	setFinalPage(true);

	_ui->pasteButton->setDefaultAction(_ui->actionPaste);

	connect(_ui->actionPaste, &QAction::triggered,
			this, &QrCodeConnectorPage::pasteQrData);
	connect(_ui->plainTextEdit, &QPlainTextEdit::textChanged,
			_viewModel, [this](){
		_viewModel->setQrData(_ui->plainTextEdit->toPlainText());
	});
	connect(_viewModel, &QrCodeConnectorViewModel::qrDataChanged,
			this, &QrCodeConnectorPage::qrDataChanged);
	connect(_viewModel->transferService(), &ClientTransferService::transferCompleted,
			qApp, &QApplication::quit);
}

bool QrCodeConnectorPage::validatePage()
{
	return _viewModel->transfer();
}

bool QrCodeConnectorPage::isComplete() const
{
	return _viewModel->isValid();
}

void QrCodeConnectorPage::pasteQrData()
{
	_ui->plainTextEdit->selectAll();
	_ui->plainTextEdit->paste();
	if(_viewModel->isValid())
		_viewModel->transfer();
}

void QrCodeConnectorPage::qrDataChanged()
{
	auto pal = palette();
	if(_viewModel->isValid()) {
		_ui->statusLabel->setText(tr("Content is valid"));
		pal.setColor(QPalette::WindowText, Qt::darkGreen);
		_ui->statusLabel->setPalette(pal);
	} else {
		_ui->statusLabel->setText(tr("Content is invalid"));
		pal.setColor(QPalette::WindowText, Qt::darkRed);
		_ui->statusLabel->setPalette(pal);
	}
	emit completeChanged();
}

QrCodeConnectorPage::~QrCodeConnectorPage() = default;
