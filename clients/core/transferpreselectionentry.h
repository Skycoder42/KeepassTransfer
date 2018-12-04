#ifndef TRANSFERPRESELECTIONENTRY_H
#define TRANSFERPRESELECTIONENTRY_H

#include <QList>
#include <QObject>

struct TransferPreSelectionEntry
{
	Q_GADGET

	Q_PROPERTY(QString key MEMBER key)
	Q_PROPERTY(bool confidential MEMBER confidential)
	Q_PROPERTY(bool selected MEMBER selected)

public:
	QString key;
	bool confidential = false;
	bool selected = false;
};

Q_DECLARE_METATYPE(TransferPreSelectionEntry)
Q_DECLARE_TYPEINFO(TransferPreSelectionEntry, Q_MOVABLE_TYPE);

#endif // TRANSFERPRESELECTIONENTRY_H
