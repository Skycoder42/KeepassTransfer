#include "transferpreselectionedit.h"
#include <QIcon>
#include <QDebug>

TransferPreSelectionEdit::TransferPreSelectionEdit(QWidget *parent) :
	QListWidget{parent}
{
	connect(this, &TransferPreSelectionEdit::itemChanged,
			this, &TransferPreSelectionEdit::updateElements);
}

QVariantList TransferPreSelectionEdit::elements() const
{
	QVariantList elements;
	elements.reserve(_elements.size());
	for(const auto &element : _elements)
		elements.append(QVariant::fromValue(element));
	return elements;
}

void TransferPreSelectionEdit::setElements(const QVariantList &elements)
{
	_elements.clear();
	_elements.reserve(elements.size());
	for(const auto &element : elements)
		_elements.append(element.value<TransferPreSelectionEntry>());

	clear();
	_blockUpdate = true;
	for(const auto &element : qAsConst(_elements)) {
		auto item = new QListWidgetItem{this};
		item->setText(element.key);
		item->setIcon(element.confidential ?
						  QIcon::fromTheme(QStringLiteral("lock")) :
						  QIcon::fromTheme(QStringLiteral("unlock")));
		item->setCheckState(element.selected ? Qt::Checked : Qt::Unchecked);
		item->setFlags((item->flags() | Qt::ItemIsUserCheckable) & ~Qt::ItemIsEditable);
	}
	_blockUpdate = false;

	emit elementsChanged();
}

void TransferPreSelectionEdit::updateElements(QListWidgetItem *item)
{
	if(_blockUpdate)
		return;

	for(auto &element : _elements) {
		if(element.key == item->text()) {
			element.selected = (item->checkState() != Qt::Unchecked);
			emit elementsChanged();
			return;
		}
	}
}
