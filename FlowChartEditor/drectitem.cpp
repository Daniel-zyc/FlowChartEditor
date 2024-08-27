#include "drectitem.h"

DRectItem::DRectItem(QGraphicsItem *parent)
	: DShapeBase(parent), rect(0, 0, 0, 0)
{

}

DRectItem::DRectItem(qreal w, qreal h, QGraphicsItem *parent)
	: DRectItem(parent)
{
	rect = QRectF(-w/2, -h/2, w, h);
}

void DRectItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	setBrush(QBrush(Qt::transparent));
	painter->setBrush(brush());
	painter->setPen(pen());
	painter->drawRect(rect);
}

QRectF DRectItem::sizeRect() const
{
	return rect;
}

QPainterPath DRectItem::shapeNormal() const
{
	QPainterPath pth;
	pth.addRect(rect);
	return pth;
}

void DRectItem::resizeToRect(QRectF nrect)
{
	rect = nrect;
}

void DRectItem::modifyToPoint(QPointF p, int id)
{
	Q_UNUSED(p); Q_UNUSED(id);
	return;
}
