#include "ditemgroup.h"

DItemGroup::DItemGroup(QGraphicsItem *parent)
	: QGraphicsItemGroup(parent)
{

}

QRectF DItemGroup::boundingRect() const
{
	return childrenBoundingRect();;
}

void DItemGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(isSelected())
	{
		prepareGeometryChange();
		painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
		painter->setBrush(Qt::NoBrush);
		painter->drawRect(boundingRect());
	}
}
