#include "drectitem.h"

DRectItem::DRectItem(QGraphicsItem *parent)
	: DShapeBase(parent), rect(0, 0, 0, 0)
{

}

DRectItem::DRectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
	: DRectItem(parent)
{
	rect = QRectF(x, y, w, h);
}

void DRectItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(rect.width() > 0 && rect.height() > 0)
	{
		painter->setBrush(brush());
		painter->setPen(pen());
		painter->drawRect(rect);
	}
}

QRectF DRectItem::sizeRect() const
{
	return rect;
}
