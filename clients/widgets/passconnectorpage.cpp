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

	QtMvvm::bind(_viewModel, "channelId",
				 _ui->channelIDLineEdit, "text",
				 QtMvvm::Binding::TwoWay,
				 nullptr,
				 "editingFinished()");
	QtMvvm::bind(_viewModel, "passphrase",
				 _ui->passphraseLineEdit, "text",
				 QtMvvm::Binding::TwoWay,
				 nullptr,
				 "editingFinished()");
	connect(_viewModel, &PassConnectorViewModel::channelIdChanged,
			this, &PassConnectorPage::completeChanged);
}

bool PassConnectorPage::validatePage()
{
	return _viewModel->transfer();
}

bool PassConnectorPage::isComplete() const
{
	return _viewModel->isValid();
}

PassConnectorPage::~PassConnectorPage() = default;
