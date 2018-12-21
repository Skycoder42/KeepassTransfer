#ifndef KPXCCLIENTIMPORTER_H
#define KPXCCLIENTIMPORTER_H

#include <QObject>
#include <QPointer>
#include <QtMvvmCore/ProgressControl>

#include <kpxcclient/client.h>

#include <credential.h>

class KPXCClientImporter : public QObject
{
	Q_OBJECT

public:
	explicit KPXCClientImporter(QObject *parent = nullptr);

public slots:
	void setKpxcPath(QString path);
	void importCredentials();

signals:
	void credentialsImported(const QList<Credential> &credentials);

private slots:
	void databaseOpen();
	void loginsReceived(const QList<KPXCClient::Entry> &entries);
	void disconnected();
	void onError(KPXCClient::Client::Error error,
				 const QString &message,
				 const QString &action,
				 bool unrecoverable);

	void onCancel();

private:
	QString _kpxcPath;
	KPXCClient::Client *_client;
	QUrl _fetchUrl;

	QPointer<QtMvvm::ProgressControl> _currentProgress;

	void convertEntry(const KPXCClient::Entry &entry);
};

#endif // KPXCCLIENTIMPORTER_H
