#include "dhexgonitem.h"
#include "../../magpoint.h"

DHexgonItem::DHexgonItem(QGraphicsItem *parent)
	: DHexgonItem(minRectSize, minRectSize, parent) {}

DHexgonItem::DHexgonItem(qreal w,qreal h, QGraphicsItem *parent)
	: DPolygonBase("", parent)
{
	modis.resize(1);
	for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
	polygon << QPointF(-w/2 * 0.6, -h/2) << QPointF(w/2 * 0.6, -h/2)
			<< QPointF(w/2, 0) << QPointF(w/2 * 0.6, h/2)
			<< QPointF(-w/2 * 0.6, h/2) << QPointF(-w/2, 0);
	updateAll();
}

void DHexgonItem::updateMagPoint()
{
	(*mags)[0]->setPos((polygon[0] + polygon[1]) / 2);
	(*mags)[1]->setPos((polygon[3] + polygon[4]) / 2);
	(*mags)[2]->setPos(polygon[2]);
	(*mags)[3]->setPos(polygon[5]);
}

void DHexgonItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(id);
	QRectF rc = sizeRect(); qreal nx = p.x();
	nx = qMin(nx, 0.0); nx = qMax(nx, rc.left());
	polygon[0].setX(nx); polygon[1].setX(-nx);
	polygon[3].setX(-nx); polygon[4].setX(nx);
	updateModiPoint();
	updateMagPoint();
}

void DHexgonItem::updateModiPoint()
{
	modis[0] = polygon[0];
}

//==============================================================================

void DHexgonItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DPolygonBase::serialize(out, fa);
}

bool DHexgonItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DPolygonBase::deserialize(in, fa)) return false;
	updateAll();
	return true;
}

