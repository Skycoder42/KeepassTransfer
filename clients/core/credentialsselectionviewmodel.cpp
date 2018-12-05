#include "credentialsselectionviewmodel.h"
#include "transferselectionviewmodel.h"
#include <QIcon>
#include <QtMvvmCore/Messages>

const QString CredentialsSelectionViewModel::paramCred = QStringLiteral("credentials");

QVariantHash CredentialsSelectionViewModel::params(const QList<Credential> &creds)
{
	return {
		{paramCred, QVariant::fromValue(creds)}
	};
}

CredentialsSelectionViewModel::CredentialsSelectionViewModel(QObject *parent) :
	ViewModel{parent},
	_credModel{new QStandardItemModel{0, 1, this}}
{
	_credModel->setItemRoleNames({
									 {KeyRole, "key"},
									 {IconRole, "icon"},
									 {SelectedRole, "selected"},
									 {IconNameRole, "iconName"},
									 {IconPathRole, "iconPath"}
								 });
}

QStandardItemModel *CredentialsSelectionViewModel::credModel() const
{
	return _credModel;
}

bool CredentialsSelectionViewModel::commitCredentials()
{
	auto creds = _credentials;
	for(auto it = creds.begin(); it != creds.end();) {
		const auto item = _modelMap.value(it->key());
		if(item && item->checkState() != Qt::Unchecked)
			++it;
		else
			it = creds.erase(it);
	}

	if(creds.isEmpty()) {
		QtMvvm::critical(tr("Invalid Credentials"),
						 tr("You must select at least one credential before you can continue!"));
		return false;
	} else {
		show<TransferSelectionViewModel>(TransferSelectionViewModel::params(creds));
		return true;
	}
}

void CredentialsSelectionViewModel::onInit(const QVariantHash &params)
{
	Q_ASSERT(_settings);
	_credentials = params.value(paramCred).value<QList<Credential>>();

	_modelMap.clear();
	_credModel->clear();

	// step 1: create all items with name and confidential info
	for(const auto &cred : qAsConst(_credentials)) {
		auto item = new QStandardItem{cred.key()};
		item->setData(cred.confidential() ?
						  QStringLiteral("lock") :
						  QStringLiteral("unlock"),
					  IconNameRole);
		item->setData(cred.confidential() ?
						  QStringLiteral(":/icons/locked.svg") :
						  QStringLiteral(":/icons/unlocked.svg"),
					  IconPathRole);
		item->setIcon(QIcon::fromTheme(item->data(IconNameRole).toString(),
									   QIcon{item->data(IconPathRole).toString()}));
		item->setCheckable(true);
		_modelMap.insert(cred.key(), item);
		_credModel->appendRow({item});
	}

	// step 2: compare with config to generate the pre-selection
	QSet<QString> knownItems;
	for(const auto &entry : _settings->preselected) {
		const auto item = _modelMap.value(entry.key);
		if(item) {
			knownItems.insert(entry.key);
			item->setCheckState(entry.selected ? Qt::Checked : Qt::Unchecked);
		}
	}

	// step 3: gather keys of all previously unknown entries
	const auto newItems = QSet<QString>::fromList(_modelMap.keys()).subtract(knownItems);
	for(const auto &item : newItems) {
		auto newEntry = _settings->preselected.push_deferred();
		newEntry->key = item;
		newEntry->selected = false;
	}
}

void CredentialsSelectionViewModel::selectAll()
{
	for(const auto item : qAsConst(_modelMap))
		item->setCheckState(Qt::Checked);
}

void CredentialsSelectionViewModel::deselectAll()
{
	for(const auto item : qAsConst(_modelMap))
		item->setCheckState(Qt::Unchecked);
}
