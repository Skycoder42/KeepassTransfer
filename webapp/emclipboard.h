#ifndef EMCLIPBOARD_H
#define EMCLIPBOARD_H

#include <QObject>
#include <QClipboard>

class EmClipboard : public QObject
{
	Q_OBJECT
public:
	explicit EmClipboard(QObject *parent = nullptr);

private slots:
	void qtDataChanged();

private:
	QClipboard * const _qtClipboard;
};

#endif // EMCLIPBOARD_H
