#ifndef TRANSFERPRESELECTIONEDIT_H
#define TRANSFERPRESELECTIONEDIT_H

#include "transferpreselectionentry.h"
#include <QListWidget>

class TransferPreSelectionEdit : public QListWidget
{
	Q_OBJECT

	Q_PROPERTY(QVariantList elements READ elements WRITE setElements NOTIFY elementsChanged USER true)

public:
	explicit TransferPreSelectionEdit(QWidget *parent = nullptr);

	QVariantList elements() const;

public slots:
	void setElements(const QVariantList &elements);

signals:
	void elementsChanged();

private slots:
	void updateElements(QListWidgetItem *item);

private:
	QList<TransferPreSelectionEntry> _elements;
	bool _blockUpdate = false;
};

#endif // TRANSFERPRESELECTIONEDIT_H
