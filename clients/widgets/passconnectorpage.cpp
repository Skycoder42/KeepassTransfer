#include "passconnectorpage.h"
#include "ui_passconnectorpage.h"
#include <QtMvvmCore/Binding>

PassConnectorPage::PassConnectorPage(QtMvvm::ViewModel *viewModel, QWidget *parent) :
	QWizardPage{parent},
	_viewModel{static_cast<PassConnectorViewModel*>(viewModel)},
	_ui{new Ui::PassConnectorPage{}}
{
	_ui->setupUi(this);
	setFinalPage(true);

	_ui->channelIDLineEdit->setValidator(_viewModel->channelIdValidator());
	_ui->passphraseLineEdit->addAction(_ui->actionGenerate_random_passphrase, QLineEdit::TrailingPosition);
	_ui->passphraseLineEdit->addAction(_ui->actionToggle_passphrase_visibility, QLineEdit::TrailingPosition);

	connect(_ui->actionToggle_passphrase_visibility, &QAction::triggered,
			this, &PassConnectorPage::toggleVisible);
	connect(_ui->actionGenerate_random_passphrase, &QAction::triggered,
			_viewModel, &PassConnectorViewModel::generateRandomPassphrase);

	QtMvvm::bind(_viewModel, "channelIdStr",
				 _ui->channelIDLineEdit, "text",
				 QtMvvm::Binding::OneWayToViewModel);
	QtMvvm::bind(_viewModel, "passphrase",
				 _ui->passphraseLineEdit, "text",
				 QtMvvm::Binding::TwoWay,
				 nullptr,
				 "editingFinished()");
	connect(_viewModel, &PassConnectorViewModel::channelIdChanged,
			this, &PassConnectorPage::appIdChanged);
	appIdChanged();
}

bool PassConnectorPage::validatePage()
{
	return _viewModel->transfer();
}

bool PassConnectorPage::isComplete() const
{
	return _viewModel->isValid();
}

void PassConnectorPage::appIdChanged()
{
	auto pal = palette();
	if(_viewModel->isValid()) {
		_ui->statusLabel->setText(tr("Channel-ID is valid"));
		pal.setColor(QPalette::WindowText, Qt::darkGreen);
		_ui->statusLabel->setPalette(pal);
	} else {
		_ui->statusLabel->setText(tr("Channel-ID is invalid"));
		pal.setColor(QPalette::WindowText, Qt::darkRed);
		_ui->statusLabel->setPalette(pal);
	}
	emit completeChanged();
}

void PassConnectorPage::toggleVisible(bool visible)
{
	_ui->passphraseLineEdit->setEchoMode(visible ? QLineEdit::Normal : QLineEdit::Password);
}

PassConnectorPage::~PassConnectorPage() = default;
