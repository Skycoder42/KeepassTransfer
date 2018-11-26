#ifndef ENCRYPTEDDATA_H
#define ENCRYPTEDDATA_H

#include <QObject>
#include <QDataStream>

class EncryptedData
{
	Q_GADGET

	Q_PROPERTY(DataMode mode MEMBER mode)
	Q_PROPERTY(QByteArray keyInfo MEMBER keyInfo)
	Q_PROPERTY(QByteArray iv MEMBER iv)
	Q_PROPERTY(QByteArray data MEMBER data)

public:
	enum class DataMode : int {
		QrCode,
		Password
	};
	Q_ENUM(DataMode)

	DataMode mode;
	QByteArray keyInfo;
	QByteArray iv;
	QByteArray data;

	bool operator==(const EncryptedData &other) const;
	bool operator!=(const EncryptedData &other) const;
};

QDataStream &operator<<(QDataStream &stream, const EncryptedData &data);
QDataStream &operator>>(QDataStream &stream, EncryptedData &data);

inline uint qHash(EncryptedData::DataMode key, uint seed) {
	return qHash(static_cast<int>(key), seed);
}

Q_DECLARE_METATYPE(EncryptedData)
Q_DECLARE_TYPEINFO(EncryptedData, Q_MOVABLE_TYPE);

#endif // ENCRYPTEDDATA_H
