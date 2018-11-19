#include "browserstorage.h"

#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/bind.h>

BrowserStorage::BrowserStorage(QObject *parent) :
	QObject{parent}
{}

int BrowserStorage::keyCount() const
{
	return EM_ASM_INT({
		var store = window.localStorage;
		console.log('before store');
		console.log(store);
		console.log('after store');
		console.log(typeof store.length);
		console.log(store.length);
		console.log('after length');
		return store.length;
	});
}

QString BrowserStorage::key(int index) const
{
	auto intPtr = EM_ASM_INT({
		var key = window.localStorage.key($0);
		var resPtr = 0;
		if(key)
			resPtr = allocate(intArrayFromString(key), 'i8', ALLOC_NORMAL);
		return resPtr;
	}, index);
	return intPtr == 0 ?
				QString{} :
				QLatin1String{reinterpret_cast<char*>(intPtr)};
}

QVariant BrowserStorage::value(const QString &key, const QVariant &defaultValue) const
{
	const auto keyData = key.toLocal8Bit();
	auto intPtr = EM_ASM_INT({
		var value = window.localStorage.getItem(Pointer_stringify($0));
		var resPtr = 0;
		if(value)
			resPtr = allocate(intArrayFromString(value), 'i8', ALLOC_NORMAL);
		return resPtr;
	}, keyData.constData());
	return intPtr == 0 ?
				defaultValue :
				QVariant{QLatin1String{reinterpret_cast<char*>(intPtr)}};
}

void BrowserStorage::setValue(const QString &key, const QVariant &value)
{
	const auto keyData = key.toLocal8Bit();
	const auto valueData = value.toString().toLocal8Bit();
	EM_ASM_ARGS({
		var key = Pointer_stringify($0);
		var value = Pointer_stringify($1);
		window.localStorage.setItem(key, value);
	}, keyData.constData(), valueData.constData());
}

void BrowserStorage::remove(const QString &key)
{
	const auto keyData = key.toLocal8Bit();
	EM_ASM_ARGS({
		var key = Pointer_stringify($0);
		window.localStorage.removeItem(key);
	}, keyData.constData());
}

void BrowserStorage::clear()
{
	EM_ASM({
		window.localStorage.clear();
	});
}
