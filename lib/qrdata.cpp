#include "qrdata.h"

QDataStream &operator<<(QDataStream &stream, const QrData &data)
{
	stream << data.channelId
		   << data.pubKey
		   << data.calcChecksum();
	return stream;
}

QDataStream &operator>>(QDataStream &stream, QrData &data)
{
	stream.startTransaction();
	quint16 checksum;
	stream >> data.channelId
		   >> data.pubKey
		   >> checksum;
	if(stream.status() != QDataStream::Ok)
		stream.rollbackTransaction();
	else if(checksum != data.calcChecksum())
		stream.abortTransaction();
	else
		stream.commitTransaction();
	return stream;
}

bool QrData::isValid() const
{
	return !channelId.isNull() && !pubKey.isEmpty();
}

QrData::operator bool() const
{
	return isValid();
}

bool QrData::operator!() const
{
	return !isValid();
}

quint16 QrData::calcChecksum() const
{
	const QByteArray checksumData = channelId.toRfc4122() + pubKey;
	return qChecksum(checksumData.data(), static_cast<uint>(checksumData.size()));
}
