#ifndef SERVERTRANSFERMESSAGE_H
#define SERVERTRANSFERMESSAGE_H

#include <QObject>
#include "kptlib.h"
#include "encrypteddata.h"

class ServerTransferMessage
{
	Q_GADGET

	Q_PROPERTY(EncryptedData data MEMBER data)

public:
	EncryptedData data;
};

Q_DECLARE_METATYPE(ServerTransferMessage)
Q_DECLARE_TYPEINFO(ServerTransferMessage, Q_MOVABLE_TYPE);

#endif // SERVERTRANSFERMESSAGE_H
