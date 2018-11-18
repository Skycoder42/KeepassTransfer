#include "encrypteddata.h"

bool EncryptedData::operator==(const EncryptedData &other) const
{
	return mode == other.mode &&
			keyInfo == other.keyInfo &&
			iv == other.iv &&
			data == other.data;
}

bool EncryptedData::operator!=(const EncryptedData &other) const
{
	return mode != other.mode ||
			keyInfo != other.keyInfo ||
			iv != other.iv ||
				  data != other.data;
}

QDataStream &operator<<(QDataStream &stream, const EncryptedData &data)
{
	stream << static_cast<int>(data.mode)
		   << data.keyInfo
		   << data.iv
		   << data.data;
	return stream;
}

QDataStream &operator>>(QDataStream &stream, EncryptedData &data)
{
	static_assert(sizeof(EncryptedData::DataMode) == sizeof(int), "EncryptedData::DataMode must be an int-enum");
	stream >> (*reinterpret_cast<int*>(&data.mode))
		   >> data.keyInfo
		   >> data.iv
		   >> data.data;
	return stream;
}
