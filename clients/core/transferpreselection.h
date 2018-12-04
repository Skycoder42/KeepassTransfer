#ifndef TRANSFERPRESELECTION_H
#define TRANSFERPRESELECTION_H

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

using TransferPreSelection = QList<TransferPreSelectionEntry>;

Q_DECLARE_METATYPE(TransferPreSelectionEntry)
Q_DECLARE_TYPEINFO(TransferPreSelectionEntry, Q_MOVABLE_TYPE);

#endif // TRANSFERPRESELECTION_H
