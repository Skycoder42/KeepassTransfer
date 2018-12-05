#include "kptsettingsviewmodel.h"
#include "settings.h"
#include "transferpreselectionentry.h"

KptSettingsViewModel::KptSettingsViewModel(QObject *parent) :
	SettingsViewModel{parent}
{}

QVariant KptSettingsViewModel::loadValue(const QString &key, const QVariant &defaultValue) const
{
	if(key == QStringLiteral("transfer/preselected")) {
		Settings settings{accessor(), nullptr};
		QVariantList presel;
		presel.reserve(settings.preselected.size());
		for(const auto &entry : settings.preselected)
			presel.append(QVariant::fromValue(TransferPreSelectionEntry{entry.key, entry.selected}));
		return presel;
	} else
		return SettingsViewModel::loadValue(key, defaultValue);
}

void KptSettingsViewModel::saveValue(const QString &key, const QVariant &value)
{
	if(key == QStringLiteral("transfer/preselected")) {
		Settings settings{accessor(), nullptr};
		settings.preselected.reset();
		for(const auto &entryVar : value.toList()) {
			const auto entry = entryVar.value<TransferPreSelectionEntry>();
			auto elem = settings.preselected.push_deferred();
			elem->key = entry.key;
			elem->selected = entry.selected;
		}
	} else
		return SettingsViewModel::saveValue(key, value);
}

void KptSettingsViewModel::resetValue(const QString &key)
{
	if(key == QStringLiteral("transfer/preselected")) {
		Settings settings{accessor(), nullptr};
		settings.preselected.reset();
		settings.preselectedSetupDummy.get();
	} else
		return SettingsViewModel::resetValue(key);
}
