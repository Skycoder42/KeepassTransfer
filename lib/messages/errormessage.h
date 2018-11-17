#ifndef ERRORMESSAGE_H
#define ERRORMESSAGE_H

#include <QObject>
#include "kptlib.h"

class ErrorMessage
{
	Q_GADGET

	Q_PROPERTY(QString message MEMBER message)

public:
	QString message;
};

Q_DECLARE_METATYPE(ErrorMessage)
Q_DECLARE_TYPEINFO(ErrorMessage, Q_MOVABLE_TYPE);

#endif // ERRORMESSAGE_H
