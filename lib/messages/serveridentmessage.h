#ifndef SERVERIDENTMESSAGE_H
#define SERVERIDENTMESSAGE_H

#include <QObject>
#include <QUuid>
#include "kptlib.h"

class ServerIdentMessage
{
	Q_GADGET

	Q_PROPERTY(QUuid channelId MEMBER channelId)

public:
	QUuid channelId;
};

Q_DECLARE_METATYPE(ServerIdentMessage)
Q_DECLARE_TYPEINFO(ServerIdentMessage, Q_MOVABLE_TYPE);

#endif // SERVERIDENTMESSAGE_H
