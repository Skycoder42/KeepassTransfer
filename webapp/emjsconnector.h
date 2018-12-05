#ifndef EMJSCONNECTOR_H
#define EMJSCONNECTOR_H

#include <QObject>
#include <QClipboard>
#include <QUrl>

class EmJsConnector : public QObject
{
	Q_OBJECT

public:
	explicit EmJsConnector(QObject *parent = nullptr);

	static EmJsConnector *instance();

	void readText();
	void updateClipboard(const QString &text);

	QUrl getHostUrl() const;

public slots:
	void setTag(const QString &tag);
	void openUrl(const QUrl &url);

	void copyText(const QString &text);

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
