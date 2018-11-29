#ifndef EMCLIPBOARD_H
#define EMCLIPBOARD_H

#include <QObject>
#include <QClipboard>

class EmClipboard : public QObject
{
	Q_OBJECT
public:
	explicit EmClipboard(QObject *parent = nullptr);

	static EmClipboard *instance();

	void readText();
	void updateClipboard(const QString &text);

private slots:
	void qtDataChanged();

	void installPasteHandler();

private:
	QClipboard * const _qtClipboard;
	bool _skipNext = false;
};

#endif // EMCLIPBOARD_H
