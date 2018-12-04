#include "transferpreselectionedit.h"
#include <QIcon>

TransferPreSelectionEdit::TransferPreSelectionEdit(QWidget *parent) :
	QListWidget{parent}
{
	connect(this, &TransferPreSelectionEdit::itemChanged,
			this, &TransferPreSelectionEdit::updateElements);
}

TransferPreSelection TransferPreSelectionEdit::elements() const
{
	return _elements;
}

void TransferPreSelectionEdit::setElements(TransferPreSelection elements)
{
	_elements = std::move(elements);
	emit elementsChanged(_elements);

	clear();
	for(const auto &element : qAsConst(_elements)) {
		auto item = new QListWidgetItem{this};
		item->setText(element.key);
		item->setIcon(element.confidential ?
						  QIcon::fromTheme(QStringLiteral("lock")) :
						  QIcon::fromTheme(QStringLiteral("unlock")));
		item->setFlags((item->flags() | Qt::ItemIsUserCheckable) & ~Qt::ItemIsEditable);
		item->setCheckState(element.selected ? Qt::Checked : Qt::Unchecked);
	}
}

void TransferPreSelectionEdit::updateElements(QListWidgetItem *item)
{
	for(auto &element : _elements) {
		if(element.key == item->text()) {
			element.selected = (item->checkState() != Qt::Unchecked);
			emit elementsChanged(_elements);
			return;
		}
	}
}
