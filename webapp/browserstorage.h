#ifndef BROWSERSTORAGE_H
#define BROWSERSTORAGE_H

#include <QObject>
#include <QVariant>
#ifndef Q_OS_WASM
#include <QSettings>
#endif

class BrowserStorage : public QObject
{
	Q_OBJECT

public:
	BrowserStorage(QObject *parent = nullptr);

	Q_INVOKABLE int keyCount() const;
	Q_INVOKABLE QString key(int index) const;
	Q_INVOKABLE QVariant value(const QString &key) const;
	Q_INVOKABLE void setValue(const QString &key, const QVariant &value);
	Q_INVOKABLE void remove(const QString &key);
	Q_INVOKABLE void clear();

private:
#ifndef Q_OS_WASM
	QSettings *_settings;
#endif
};

#endif // BROWSERSTORAGE_H
