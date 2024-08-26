#include "dellitem.h"

DEllItem::DEllItem(QGraphicsItem *parent)
	: DShapeBase(parent), rect(0, 0, 0, 0)
{

}

DEllItem::DEllItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
	: DEllItem(parent)
{
	rect = QRectF(x, y, w, h);
}

void DEllItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(rect.width() > 0 && rect.height() > 0)
	{
		painter->setBrush(brush());
		painter->setPen(pen());
		painter->drawEllipse(rect);
	}
}

QRectF DEllItem::sizeRect() const
{
	return rect;
}

QPainterPath DEllItem::shapeNormal() const
{
	QPainterPath pth;
	pth.addEllipse(rect);
	return pth;
}
