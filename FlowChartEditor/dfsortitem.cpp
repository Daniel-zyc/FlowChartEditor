#include "dfsortitem.h"
#include "magpoint.h"

DFSortItem::DFSortItem(QGraphicsItem *parent)
    : DFSortItem(minRectSize, minRectSize, parent) {}

DFSortItem::DFSortItem(qreal w, qreal h, QGraphicsItem *parent)
    : DPolygonBase("", parent)
{
    for(int i = 0; i < 8; i++) mags->push_back(new MagPoint(this));
    polygon  << QPointF(w/2, 0)<< QPointF(0, h/2) << QPointF(-w/2, 0) << QPointF(0, -h/2) << QPointF(w/2, 0)<< QPointF(-w/2, 0);
    updateAll();
}

void DFSortItem::modiToPoint(QPointF p, int id)
{
    Q_UNUSED(p); Q_UNUSED(id); return;
}

void DFSortItem::updateMagPoint()
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

void DFSortItem::updateModiPoint()
{
    return;
}

//==============================================================================

void DFSortItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
    DPolygonBase::serialize(out, fa);
}

bool DFSortItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
    if(!DPolygonBase::deserialize(in, fa)) return false;
    updateAll();
    return true;
}
