#include "browserstorage.h"

#include <emscripten/val.h>
using namespace emscripten;

BrowserStorage::BrowserStorage(QObject *parent) :
	QObject{parent}
{}

int BrowserStorage::keyCount() const
{
	auto store = val::global("localStorage");
	Q_ASSERT(!store.isNull());
	return store["length"].as<int>();
}

QString BrowserStorage::key(int index) const
{
	auto store = val::global("localStorage");
	Q_ASSERT(!store.isNull());
	const auto key = store.call<std::string>("key", index);
	return QString::fromStdString(key);
}

QVariant BrowserStorage::value(const QString &key, const QVariant &defaultValue) const
{
	auto store = val::global("localStorage");
	Q_ASSERT(!store.isNull());
	const auto value = store.call<val>("getItem", key.toStdString());
	if(value.isNull())
		return defaultValue;
	else
		return QString::fromStdString(value.as<std::string>());
}

void BrowserStorage::setValue(const QString &key, const QVariant &value)
{
	auto store = val::global("localStorage");
	Q_ASSERT(!store.isNull());
	store.call<void>("setItem", key.toStdString(), value.toString().toStdString());
}

void BrowserStorage::remove(const QString &key)
{
	auto store = val::global("localStorage");
	Q_ASSERT(!store.isNull());
	store.call<void>("removeItem", key.toStdString());
}

void BrowserStorage::clear()
{
	auto store = val::global("localStorage");
	Q_ASSERT(!store.isNull());
	store.call<void>("clear");
}
