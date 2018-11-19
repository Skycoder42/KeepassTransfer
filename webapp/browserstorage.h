#ifndef BROWSERSTORAGE_H
#define BROWSERSTORAGE_H

#include <QObject>
#include <QVariant>

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
};

#endif // BROWSERSTORAGE_H
