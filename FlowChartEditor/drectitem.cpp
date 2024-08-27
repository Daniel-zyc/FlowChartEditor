#include "drectitem.h"

DRectItem::DRectItem(QGraphicsItem *parent)
	: DShapeBase(parent), rect(0, 0, 0, 0)
{

}

DRectItem::DRectItem(qreal w, qreal h, QGraphicsItem *parent)
	: DRectItem(parent)
{
	rect = QRectF(-w/2, -h/2, w, h);

	mags.push_back(MagPoint(this));
	mags.push_back(MagPoint(this));
	mags.push_back(MagPoint(this));
	mags.push_back(MagPoint(this));
	updateMagPoint();
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

void DRectItem::updateMagPoint()
{
	mags[0].x = rect.left();
	mags[0].y = 0;
	mags[1].x = rect.right();
	mags[1].y = 0;

	mags[2].x = 0;
	mags[2].y = rect.top();
	mags[3].x = 0;
	mags[3].y = rect.bottom();
}

void DRectItem::resizeToRect(QRectF nrect)
{
	rect = nrect;
	updateMagPoint();
}

void DRectItem::modifyToPoint(QPointF p, int id)
{
	Q_UNUSED(p); Q_UNUSED(id);
	return;
}
