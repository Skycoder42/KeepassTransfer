#ifndef QRDATA_H
#define QRDATA_H

#include <QObject>
#include <QUuid>
#include <QDataStream>

class QrData
{
	Q_GADGET

	Q_PROPERTY(QUuid channelId MEMBER channelId)
	Q_PROPERTY(QByteArray pubKey MEMBER pubKey)

	Q_PROPERTY(bool valid READ isValid STORED false)

public:
	QUuid channelId;
	QByteArray pubKey;

	bool isValid() const;
	explicit operator bool() const;
	bool operator!() const;

private:
	friend QDataStream &operator<<(QDataStream &stream, const QrData &data);
	friend QDataStream &operator>>(QDataStream &stream, QrData &data);

	quint16 calcChecksum() const;
};

QDataStream &operator<<(QDataStream &stream, const QrData &data);
QDataStream &operator>>(QDataStream &stream, QrData &data);

Q_DECLARE_METATYPE(QrData)
Q_DECLARE_TYPEINFO(QrData, Q_MOVABLE_TYPE);

#endif // QRDATA_H
