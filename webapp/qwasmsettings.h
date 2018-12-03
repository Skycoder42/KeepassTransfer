#ifndef QWASMSETTINGS_H
#define QWASMSETTINGS_H

#include <QSettings>

class QWasmSettings
{
	Q_DISABLE_COPY(QWasmSettings)
public:
	QWasmSettings() = delete;

	static QSettings::Format registerFormat();
	static void prepareFile(const QString &path);

private:
	static bool readWasmSettings(QIODevice &device, QSettings::SettingsMap &map);
	static bool writeWasmSettings(QIODevice &device, const QSettings::SettingsMap &map);
};

#endif // QWASMSETTINGS_H
