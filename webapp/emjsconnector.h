#ifndef EMJSCONNECTOR_H
#define EMJSCONNECTOR_H

#include <QObject>
#include <QClipboard>

class EmJsConnector : public QObject
{
	Q_OBJECT

public:
	explicit EmJsConnector(QObject *parent = nullptr);

	static EmJsConnector *instance();

	void readText();
	void updateClipboard(const QString &text);

	Q_INVOKABLE void setTag(const QString &tag);

signals:
	void tagChanged(const QString &tag);

private slots:
	void qtDataChanged();

	void installHandlers();

private:
	QClipboard * const _qtClipboard = nullptr;
	bool _skipNext = false;
};

#endif // EMJSCONNECTOR_H
