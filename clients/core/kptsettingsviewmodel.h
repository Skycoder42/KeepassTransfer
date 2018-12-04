#ifndef KPTSETTINGSVIEWMODEL_H
#define KPTSETTINGSVIEWMODEL_H

#include <QtMvvmCore/SettingsViewModel>

class KptSettingsViewModel : public QtMvvm::SettingsViewModel
{
	Q_OBJECT

public:
	Q_INVOKABLE explicit KptSettingsViewModel(QObject *parent = nullptr);

	QVariant loadValue(const QString &key, const QVariant &defaultValue) const override;
	void saveValue(const QString &key, const QVariant &value) override;
	void resetValue(const QString &key) override;
};

#endif // KPTSETTINGSVIEWMODEL_H
