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

PassConnectorPage::~PassConnectorPage() = default;
