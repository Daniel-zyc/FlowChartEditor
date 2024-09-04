#include "dfcompareitem.h"
#include "magpoint.h"

DFCompareItem::DFCompareItem(QGraphicsItem *parent)
    : DFCompareItem(minRectSize, minRectSize, parent) {}

DFCompareItem::DFCompareItem(qreal w, qreal h, QGraphicsItem *parent)
    : DPolygonBase("", parent)
{
    for(int i = 0; i < 8; i++) mags->push_back(new MagPoint(this));
    polygon  << QPointF(0, 0)<< QPointF(w/2, -h/2) << QPointF(-w/2,-h/2) << QPointF(w/2, h/2) << QPointF(-w/2, h/2);
    updateAll();
}

void DFCompareItem::modiToPoint(QPointF p, int id)
{
    Q_UNUSED(p); Q_UNUSED(id); return;
}

void DFCompareItem::updateMagPoint()
{
    (*mags)[0]->setPos(polygon[0]);
    (*mags)[1]->setPos((polygon[1] + polygon[2]) / 2);
    (*mags)[2]->setPos((polygon[3] + polygon[4]) / 2);
}

void DFCompareItem::updateModiPoint()
{
    return;
}

//==============================================================================

void DFCompareItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
    DPolygonBase::serialize(out, fa);
}

bool DFCompareItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
    if(!DPolygonBase::deserialize(in, fa)) return false;
    updateAll();
    return true;
}
