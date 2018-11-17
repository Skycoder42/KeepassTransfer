#ifndef APPIDENTMESSAGE_H
#define APPIDENTMESSAGE_H

#include <QObject>
#include "kptlib.h"

class AppIdentMessage
{
	Q_GADGET

	Q_PROPERTY(quint32 version MEMBER version)

public:
	quint32 version = KPTLib::ProtocolVersion;
};

Q_DECLARE_METATYPE(AppIdentMessage)
Q_DECLARE_TYPEINFO(AppIdentMessage, Q_MOVABLE_TYPE);

#endif // APPIDENTMESSAGE_H
