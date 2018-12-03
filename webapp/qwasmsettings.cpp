#include "qwasmsettings.h"
#include <QDir>
#include <QUuid>
#include <QDebug>

#include <emscripten/val.h>
using namespace emscripten;

QSettings::Format QWasmSettings::registerFormat()
{
	const auto format = QSettings::registerFormat(QStringLiteral("wasmconf"),
												  &QWasmSettings::readWasmSettings,
												  &QWasmSettings::writeWasmSettings,
												  Qt::CaseSensitive);
	QSettings::setPath(format, QSettings::UserScope, QDir::tempPath());
	QSettings::setPath(format, QSettings::SystemScope, QDir::tempPath());
	QSettings::setDefaultFormat(format);

	QSettings mPathOrigin;
	prepareFile(mPathOrigin.fileName());
	return format;
}

void QWasmSettings::prepareFile(const QString &path)
{
	QFileInfo{path}.dir().mkpath(QStringLiteral("."));
	QFile sFile{path};
	if(!sFile.open(QIODevice::WriteOnly)) {
		qWarning() << "QWasmSettings: Failed to prepare path"
				   << path << "with error:" << qUtf8Printable(sFile.errorString());
		return;
	}

	sFile.write(QUuid::createUuid().toByteArray());
	sFile.close();
}

bool QWasmSettings::readWasmSettings(QIODevice &device, QSettings::SettingsMap &map)
{
	Q_UNUSED(device);

	auto store = val::global("localStorage");
	if(store.isNull())
		return false;

	const auto keyCount = store["length"].as<int>();
	for(auto i = 0; i < keyCount; ++i) {
		const auto key = store.call<std::string>("key", i);
		const auto value = store.call<val>("getItem", key).as<std::string>();
		map.insert(QString::fromStdString(key),
				   QString::fromStdString(value));
	}

	return true;
}

bool QWasmSettings::writeWasmSettings(QIODevice &device, const QSettings::SettingsMap &map)
{
	auto store = val::global("localStorage");
	if(store.isNull())
		return false;

	store.call<void>("clear");
	for(auto it = map.constBegin(); it != map.constEnd(); ++it) {
		store.call<void>("setItem",
						 it.key().toStdString(),
						 it.value().toString().toStdString());
	}

	device.write(QUuid::createUuid().toByteArray());
	return true;
}
