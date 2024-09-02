#include "dtriitem.h"
#include "magpoint.h"

DTriItem::DTriItem(QGraphicsItem *parent)
	: DTriItem(minRectSize, minRectSize, parent) {}

DTriItem::DTriItem(qreal w, qreal h, QGraphicsItem *parent)
	: DPolygonBase(parent)
{
	modis.resize(1);
	for(int i = 0; i < 3; i++) mags->push_back(new MagPoint(this));

	polygon << QPointF(0, -h/2) << QPointF(w/2, h/2) << QPointF(-w/2, h/2);
	updateAll();
}

void DTriItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(id);
	QRectF rc = sizeRect();
	p.setX(qMax(p.x(), rc.left()));
	p.setX(qMin(p.x(), rc.right()));
	polygon[0].setX(p.x());
	updateAll();
}

void DTriItem::updateMagPoint()
{
	(*mags)[0]->setPos((polygon[0] + polygon[1]) / 2);
	(*mags)[1]->setPos((polygon[1] + polygon[2]) / 2);
	(*mags)[2]->setPos((polygon[2] + polygon[0]) / 2);
}

void DTriItem::updateModiPoint()
{
	modis[0] = polygon[0];
}

//==============================================================================

void DTriItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DPolygonBase::serialize(out, fa);
}

bool DTriItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DPolygonBase::deserialize(in, fa)) return false;
	updateAll();
	return true;
}
