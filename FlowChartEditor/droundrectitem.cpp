#include "droundrectitem.h"
#include "magpoint.h"

DRoundRectItem::DRoundRectItem(QGraphicsItem *parent)
	: DRoundRectItem(minRectSize, minRectSize, parent) {}

DRoundRectItem::DRoundRectItem(qreal w, qreal h, QGraphicsItem *parent)
	: DShapeBase("", parent)
{
	modis.resize(2);
	for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
	rect = QRectF(-w/2, -h/2, w, h);
	updateAll();
}

void DRoundRectItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

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
	QPainterPath pth; pth.addRoundedRect(rect, radiusx, radiusy); return pth;
}

void DRoundRectItem::updateMagPoint()
{
	(*mags)[0]->setPos({rect.left(), 0});
	(*mags)[1]->setPos({rect.right(), 0});

	(*mags)[2]->setPos({0, rect.top()});
	(*mags)[3]->setPos({0, rect.bottom()});
}

void DRoundRectItem::updateModiPoint()
{
	radiusx = qMin(radiusx, rect.width() / 2); radiusx = qMax(0.0, radiusx);
	radiusy = qMin(radiusy, rect.height() / 2); radiusy = qMax(0.0, radiusy);

	modis[0] = {rect.left() + radiusx, rect.top()};
	modis[1] = {rect.left(), rect.top() + radiusy};
}

void DRoundRectItem::sizeToRect(QRectF nrect)
{
	rect = nrect; updateAll();
}

void DRoundRectItem::modiToPoint(QPointF p, int id)
{
	switch(id)
	{
		case 0:
			radiusx = qAbs(rect.left() - p.x());
			updateModiPoint();
			break;
		case 1:
			radiusy = qAbs(rect.top() - p.y());
			updateModiPoint();
			break;
	}
	return;
}

void DRoundRectItem::updateAll()
{
	updateSizePoint();
	updateMagPoint();
	updateModiPoint();
}

//====================================

void DRoundRectItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DShapeBase::serialize(out, fa);

	out << rect << radiusx << radiusy;
}

bool DRoundRectItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DShapeBase::deserialize(in, fa)) return false;

	in >> rect >> radiusx >> radiusy;
	updateAll();
	return true;
}
