#include "droundrectitem.h"

DRoundRectItem::DRoundRectItem(QGraphicsItem *parent)
	: DShapeBase(parent)
{
}

DRoundRectItem::DRoundRectItem(qreal w, qreal h, QGraphicsItem *parent)
	: DRoundRectItem(parent)
{
	rect = QRectF(-w/2, -h/2, w, h);
	radiusx = 30; radiusy = 30;

	modis.resize(2);
	updateModiPoint();

	mags.push_back(MagPoint(this));
	mags.push_back(MagPoint(this));
	mags.push_back(MagPoint(this));
	mags.push_back(MagPoint(this));
	updateMagPoint();
}

void DRoundRectItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	setBrush(QBrush(Qt::transparent));
	painter->setBrush(brush());
	painter->setPen(pen());
	painter->drawRoundedRect(rect, radiusx, radiusy);
}

QRectF DRoundRectItem::sizeRect() const
{
	return rect;
}

QPainterPath DRoundRectItem::shapeNormal() const
{
	QPainterPath pth;
	pth.addRect(rect);
	return pth;
}

void DRoundRectItem::updateMagPoint()
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

void DRoundRectItem::updateModiPoint()
{
	modis[0].setX(rect.left() + radiusx);
	modis[0].setY(rect.top());

	modis[1].setX(rect.left());
	modis[1].setY(rect.top() + radiusy);
}

void DRoundRectItem::resizeToRect(QRectF nrect)
{
	rect = nrect;
	updateModiPoint();
	updateMagPoint();
}

void DRoundRectItem::modifyToPoint(QPointF p, int id)
{
	switch(id)
	{
		case 0:
			radiusx = qAbs(rect.left() - p.x());
			radiusx = qMin(radiusx, rect.width() / 2);
			updateModiPoint();
			break;
		case 1:
			radiusy = qAbs(rect.top() - p.y());
			radiusy = qMin(radiusy, rect.height() / 2);
			updateModiPoint();
			break;
	}

	return;
}
