#ifndef QRCODESCANNER_H
#define QRCODESCANNER_H

#include <QObject>
#include <QPointer>

class QrCodeScanner : public QObject
{
	Q_OBJECT

	Q_PROPERTY(bool canScan READ canScan CONSTANT)

public:
	explicit QrCodeScanner(QObject *parent = nullptr);

	static QrCodeScanner *instance();

	bool canScan() const;

public slots:
	void initiateScan();

signals:
	void scanResultReady(const QString &scanResult);

private:
	static QPointer<QrCodeScanner> _instance;
};

#endif // QRCODESCANNER_H
