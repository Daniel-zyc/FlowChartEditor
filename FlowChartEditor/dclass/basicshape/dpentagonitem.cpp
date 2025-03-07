#include "dpentagonitem.h"
#include "../../magpoint.h"

DPentagonItem::DPentagonItem(QGraphicsItem *parent)
    : DPentagonItem(minRectSize, minRectSize, parent) {}

DPentagonItem::DPentagonItem(qreal w, qreal h, QGraphicsItem *parent)
    : DPolygonBase("", parent)
{
    for(int i = 0; i < 5; i++) mags->push_back(new MagPoint(this));
    polygon << QPointF(0, -h/2) << QPointF(w/2, -0.12*h) << QPointF(0.309*w, h/2) << QPointF(-0.309*w, h/2)<< QPointF(-w/2, -0.12*h);
    updateAll();
}

void DPentagonItem::modiToPoint(QPointF p, int id)
{
    Q_UNUSED(p); Q_UNUSED(id); return;
}

void DPentagonItem::updateMagPoint()
{
    (*mags)[0]->setPos(polygon[0]);
    (*mags)[1]->setPos(polygon[1]);
    (*mags)[2]->setPos(polygon[2]);
    (*mags)[3]->setPos(polygon[3]);
    (*mags)[4]->setPos(polygon[4]);
}

void DPentagonItem::updateModiPoint()
{
    return;
}

//==============================================================================

void DPentagonItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
    DPolygonBase::serialize(out, fa);
}

bool DPentagonItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
    if(!DPolygonBase::deserialize(in, fa)) return false;
    updateAll();
    return true;
}
