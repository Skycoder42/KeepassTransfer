#ifndef QRCODESCANNER_H
#define QRCODESCANNER_H

#include <QObject>

class QrCodeScanner : public QObject
{
	Q_OBJECT

public:
	explicit QrCodeScanner(QObject *parent = nullptr);

public slots:
	void initiateScan();

signals:
	void scanResultReady(const QString &data);
};

#endif // QRCODESCANNER_H
