#include "credentialseditviewmodel.h"
#include "transferselectionviewmodel.h"
#include <QtMvvmCore/Messages>
#ifdef USE_KPXCCLIENT_LIB
#include "kpxcclientimporter.h"
#endif

CredentialsEditViewModel::CredentialsEditViewModel(QObject *parent) :
	ViewModel{parent},
	_credModel{new CredentialsModel{this}}
#ifdef USE_KPXCCLIENT_LIB
	,_importer{new KPXCClientImporter{this}}
#endif
{
	setupModel();
	addEmptyEntry();

#ifdef USE_KPXCCLIENT_LIB
	connect(_importer, &KPXCClientImporter::credentialsImported,
			this, &CredentialsEditViewModel::entryImported);
#endif
}

CredentialsEditViewModel::CredentialsModel *CredentialsEditViewModel::credentialsModel() const
{
	return _credModel;
}

bool CredentialsEditViewModel::commitCredentials()
{
	auto creds = _credModel->gadgets();
	Credential::filterOutEmpty(creds);
	if(creds.isEmpty()) {
		QtMvvm::critical(tr("Invalid Credentials"),
						 tr("You must enter at least one credential before you can continue!"));
		return false;
	} else {
		show<TransferSelectionViewModel>(TransferSelectionViewModel::params(creds));
		return true;
	}
}

void CredentialsEditViewModel::addEmptyEntry()
{
	_credModel->addGadget({});
}

void CredentialsEditViewModel::importFromKPXC()
{
#ifdef USE_KPXCCLIENT_LIB
	_importer->importCredentials();
#else
	Q_UNIMPLEMENTED();
#endif
}

void CredentialsEditViewModel::entryImported(const QList<Credential> &credentials)
{
	_credModel->resetModel(credentials);
}

void CredentialsEditViewModel::setupModel()
{
	using Convert = CredentialsModel::Convert;

	_credModel->setEditable(true);

	// key column
	auto column = _credModel->addColumn(tr("Key"), "key");
	_credModel->addRole(column, Qt::EditRole, "key");

	// value column
	column = _credModel->addColumn(tr("Value"), "value");
	_credModel->addRole(column, Qt::EditRole, "value");
	_credModel->addAliasConverter(column, Qt::DisplayRole, [this](Convert c, const QVariant &value) {
		if(c == Convert::Read) {
			for(const auto &cred : _credModel->gadgets()) { // NOTE uneffective, make better...
				if(cred.value() == value && cred.confidential())
					return QVariant{QStringLiteral("●").repeated(value.toString().size())};
			}
			return value;
		} else
			return QVariant{};
	});

	// confidential column
	column = _credModel->addColumn(tr("Confidential"), "confidential");
	_credModel->addRole(column, Qt::CheckStateRole, "confidential");
	_credModel->setExtraFlags(column, Qt::ItemIsUserCheckable, Qt::ItemIsEditable);
	_credModel->addAliasConverter(column, Qt::DisplayRole, [](Convert c, const QVariant &value){
		if(c == Convert::Read)
			return QVariant{value.toBool() ? tr("Confidential") : tr("Common")};
		else
			return QVariant{};
	});
	_credModel->addAliasConverter(column, Qt::CheckStateRole, [](Convert c, const QVariant &value){
		if(c == Convert::Read)
			return QVariant::fromValue(value.toBool() ? Qt::Checked : Qt::Unchecked);
		else if(c == Convert::Write)
			return QVariant{value.value<Qt::CheckState>() != Qt::Unchecked};
		else
			Q_UNREACHABLE();
	});
}
