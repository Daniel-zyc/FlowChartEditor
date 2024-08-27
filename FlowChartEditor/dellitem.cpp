#include "dellitem.h"

DEllItem::DEllItem(QGraphicsItem *parent)
	: DShapeBase(parent), rect(0, 0, 0, 0)
{

}

DEllItem::DEllItem(qreal w, qreal h, QGraphicsItem *parent)
	: DEllItem(parent)
{
	rect = QRectF(-w/2, -h/2, w, h);

	mags.push_back(MagPoint(this));
	mags.push_back(MagPoint(this));
	mags.push_back(MagPoint(this));
	mags.push_back(MagPoint(this));
	updateMagPoint();
}

void DEllItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	setBrush(QBrush(Qt::transparent));
	painter->setBrush(brush());
	painter->setPen(pen());
	painter->drawEllipse(rect);
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

void DEllItem::updateMagPoint()
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

void DEllItem::resizeToRect(QRectF nrect)
{
	rect = nrect;
	updateMagPoint();
}

void DEllItem::modifyToPoint(QPointF p, int id)
{
	Q_UNUSED(p); Q_UNUSED(id);
	return;
}
