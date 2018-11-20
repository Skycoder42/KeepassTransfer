#ifndef QRIMAGEPROVIDER_H
#define QRIMAGEPROVIDER_H

#include <QQuickImageProvider>

class QrImageProvider : public QQuickImageProvider
{
public:
	QrImageProvider();

	QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
};

#endif // QRIMAGEPROVIDER_H
