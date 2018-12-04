#ifndef TRANSFERPRESELECTIONEDIT_H
#define TRANSFERPRESELECTIONEDIT_H

#include "transferpreselection.h"
#include <QListWidget>

class TransferPreSelectionEdit : public QListWidget
{
	Q_OBJECT

	Q_PROPERTY(TransferPreSelection elements READ elements WRITE setElements NOTIFY elementsChanged USER true)

public:
	explicit TransferPreSelectionEdit(QWidget *parent = nullptr);

	TransferPreSelection elements() const;

public slots:
	void setElements(TransferPreSelection elements);

signals:
	void elementsChanged(TransferPreSelection elements);

private slots:
	void updateElements(QListWidgetItem *item);

private:
	TransferPreSelection _elements;
};

#endif // TRANSFERPRESELECTIONEDIT_H
