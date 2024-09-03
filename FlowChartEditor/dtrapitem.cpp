#include "dtrapitem.h"
#include "magpoint.h"

DTrapItem::DTrapItem(QGraphicsItem *parent)
	: DTrapItem(minRectSize, minRectSize, parent) {}

DTrapItem::DTrapItem(qreal w,qreal h, QGraphicsItem *parent)
	: DPolygonBase("", parent)
{
	modis.resize(1);
	for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
	polygon << QPointF(-w/4, -h/2) << QPointF(w/4, -h/2)
			<< QPointF(w/2, h/2) << QPointF(-w/2, h/2);
	updateAll();
}

void DTrapItem::updateMagPoint()
{
	(*mags)[0]->setPos((polygon[0] + polygon[1]) / 2);
	(*mags)[1]->setPos((polygon[1] + polygon[2]) / 2);
	(*mags)[2]->setPos((polygon[2] + polygon[3]) / 2);
	(*mags)[3]->setPos((polygon[3] + polygon[0]) / 2);
}

void DTrapItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(id);
	QRectF rc = sizeRect(); qreal nx = p.x();
	nx = qMin(nx, 0.0); nx = qMax(nx, rc.left());
	polygon[0].setX(nx); polygon[1].setX(-nx);
	updateModiPoint();
	updateMagPoint();
}

void DTrapItem::updateModiPoint()
{
	modis[0] = polygon[0];
}

//==============================================================================

void DTrapItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DPolygonBase::serialize(out, fa);
}

bool DTrapItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DPolygonBase::deserialize(in, fa)) return false;
	updateAll();
	return true;
}
