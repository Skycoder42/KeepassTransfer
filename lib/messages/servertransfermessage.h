#ifndef SERVERTRANSFERMESSAGE_H
#define SERVERTRANSFERMESSAGE_H

#include <QObject>
#include "kptlib.h"

class ServerTransferMessage
{
	Q_GADGET

	Q_PROPERTY(QByteArray data MEMBER data)

public:
	QByteArray data;
};

Q_DECLARE_METATYPE(ServerTransferMessage)
Q_DECLARE_TYPEINFO(ServerTransferMessage, Q_MOVABLE_TYPE);

#endif // SERVERTRANSFERMESSAGE_H
