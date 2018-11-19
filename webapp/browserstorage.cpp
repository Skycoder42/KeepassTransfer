#include "browserstorage.h"

BrowserStorage::BrowserStorage(QObject *parent) :
	QObject{parent},
	_settings{new QSettings{this}}
{}

int BrowserStorage::keyCount() const
{
	return _settings->allKeys().size();
}

QString BrowserStorage::key(int index) const
{
	return _settings->allKeys().value(index);
}

QVariant BrowserStorage::value(const QString &key, const QVariant &defaultValue) const
{
	return _settings->value(key, defaultValue);
}

void BrowserStorage::setValue(const QString &key, const QVariant &value)
{
	_settings->setValue(key, value);
}

void BrowserStorage::remove(const QString &key)
{
	_settings->remove(key);
}

void BrowserStorage::clear()
{
	_settings->clear();
}
