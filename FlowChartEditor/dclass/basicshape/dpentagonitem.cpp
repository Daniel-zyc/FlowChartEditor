#include "dpentagonitem.h"
#include "../../magpoint.h"

pentagonItem::pentagonItem(QGraphicsItem *parent)
    : pentagonItem(minRectSize, minRectSize, parent) {}

pentagonItem::pentagonItem(qreal w, qreal h, QGraphicsItem *parent)
    : DPolygonBase("", parent)
{
    for(int i = 0; i < 5; i++) mags->push_back(new MagPoint(this));
    polygon << QPointF(0, -h/2) << QPointF(w/2, -0.12*h) << QPointF(0.309*w, h/2) << QPointF(-0.309*w, h/2)<< QPointF(-w/2, -0.12*h);
    updateAll();
}

void pentagonItem::modiToPoint(QPointF p, int id)
{
    Q_UNUSED(p); Q_UNUSED(id); return;
}

void pentagonItem::updateMagPoint()
{

    (*mags)[0]->setPos(polygon[0]);
    (*mags)[1]->setPos(polygon[1]);
    (*mags)[2]->setPos(polygon[2]);
    (*mags)[3]->setPos(polygon[3]);
    (*mags)[4]->setPos(polygon[4]);
}

void pentagonItem::updateModiPoint()
{
    return;
}

//==============================================================================

void pentagonItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
    DPolygonBase::serialize(out, fa);
}

bool pentagonItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
    if(!DPolygonBase::deserialize(in, fa)) return false;
    updateAll();
    return true;
}
