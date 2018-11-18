#ifndef CLIENTTRANSFERMESSAGE_H
#define CLIENTTRANSFERMESSAGE_H

#include <QObject>
#include <QUuid>
#include "kptlib.h"
#include "encrypteddata.h"

class ClientTransferMessage
{
	Q_GADGET

	Q_PROPERTY(quint32 version MEMBER version)
	Q_PROPERTY(QUuid channelId MEMBER channelId)
	Q_PROPERTY(EncryptedData data MEMBER data)

public:
	quint32 version = KPTLib::ProtocolVersion;
	QUuid channelId;
	EncryptedData data;
};

Q_DECLARE_METATYPE(ClientTransferMessage)
Q_DECLARE_TYPEINFO(ClientTransferMessage, Q_MOVABLE_TYPE);

#endif // CLIENTTRANSFERMESSAGE_H
