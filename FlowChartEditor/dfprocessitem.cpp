#include "dfprocessitem.h"
#include "magpoint.h"

DFProcessItem::DFProcessItem(QGraphicsItem *parent)
	: DFProcessItem(minRectSize, minRectSize, parent) {}

DFProcessItem::DFProcessItem(qreal w,qreal h, QGraphicsItem *parent)
	: DPolygonBase("", parent)
{
	for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
	polygon << QPointF(-w/2, -h/2) << QPointF(w/2, -h/2)
			<< QPointF(w/2, h/2) << QPointF(-w/2, h/2);
	updateAll();
}

void DFProcessItem::updateMagPoint()
{
	(*mags)[0]->setPos((polygon[0] + polygon[1]) / 2);
	(*mags)[1]->setPos((polygon[1] + polygon[2]) / 2);
	(*mags)[2]->setPos((polygon[2] + polygon[3]) / 2);
	(*mags)[3]->setPos((polygon[3] + polygon[0]) / 2);
}

void DFProcessItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(p); Q_UNUSED(id); return;
}

void DFProcessItem::updateModiPoint()
{
	return;
}

//==============================================================================

void DFProcessItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DPolygonBase::serialize(out, fa);
}

bool DFProcessItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DPolygonBase::deserialize(in, fa)) return false;
	updateAll();
	return true;
}

