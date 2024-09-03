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
	painter->drawRoundedRect(rect, rect.width() * radiusxRatio,
							 rect.height() * radiusyRatio);
}

QRectF DRoundRectItem::sizeRect() const
{
	return rect;
}

QPainterPath DRoundRectItem::shapeNormal() const
{
	QPainterPath pth;
	pth.addRoundedRect(rect, rect.width() * radiusxRatio,
							 rect.height() * radiusyRatio);
	return pth;
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
	modis[0] = {rect.left() + rect.width() * radiusxRatio, rect.top()};
	modis[1] = {rect.left(), rect.top() + rect.height() * radiusyRatio};
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
			radiusxRatio = (p.x() - rect.left()) / rect.width();
			radiusxRatio = qMin(radiusxRatio, 0.5);
			radiusxRatio = qMax(0.0, radiusxRatio);
			updateModiPoint();
			break;
		case 1:
			radiusyRatio = (p.y() - rect.top()) / rect.height();
			radiusyRatio = qMin(radiusyRatio, 0.5);
			radiusyRatio = qMax(0.0, radiusyRatio);
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

	out << rect << radiusxRatio << radiusyRatio;
}

bool DRoundRectItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DShapeBase::deserialize(in, fa)) return false;

	in >> rect >> radiusxRatio >> radiusyRatio;
	updateAll();
	return true;
}
