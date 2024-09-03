#include "dfconditionitem.h"
#include "magpoint.h"

DFConditionItem::DFConditionItem(QGraphicsItem *parent)
	: DFConditionItem(minRectSize, minRectSize, parent) {}

DFConditionItem::DFConditionItem(qreal w, qreal h, QGraphicsItem *parent)
	: DPolygonBase("", parent)
{
	for(int i = 0; i < 8; i++) mags->push_back(new MagPoint(this));
	polygon << QPointF(0, -h/2) << QPointF(w/2, 0) << QPointF(0, h/2) << QPointF(-w/2, 0);
	updateAll();
}

void DFConditionItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(p); Q_UNUSED(id); return;
}

void DFConditionItem::updateMagPoint()
{
	(*mags)[0]->setPos((polygon[0] + polygon[1]) / 2);
	(*mags)[1]->setPos((polygon[1] + polygon[2]) / 2);
	(*mags)[2]->setPos((polygon[2] + polygon[3]) / 2);
	(*mags)[3]->setPos((polygon[3] + polygon[0]) / 2);

	(*mags)[4]->setPos(polygon[0]);
	(*mags)[5]->setPos(polygon[1]);
	(*mags)[6]->setPos(polygon[2]);
	(*mags)[7]->setPos(polygon[3]);
}

void DFConditionItem::updateModiPoint()
{
	return;
}

//==============================================================================

void DFConditionItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DPolygonBase::serialize(out, fa);
}

bool DFConditionItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DPolygonBase::deserialize(in, fa)) return false;
	updateAll();
	return true;
}
