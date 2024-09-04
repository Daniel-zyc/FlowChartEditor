#include "dfcarditem.h"
#include "../../magpoint.h"

DFCardItem::DFCardItem(QGraphicsItem *parent)
    : DFCardItem(minRectSize, minRectSize, parent) {}

DFCardItem::DFCardItem(qreal w,qreal h, QGraphicsItem *parent)
    : DPolygonBase("", parent)
{
    modis.resize(2);
    for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
    polygon << QPointF(-w/2, -0.3*h) << QPointF(-0.3*w, -h/2) << QPointF(w/2, -h/2)
            << QPointF(w/2, h/2) << QPointF(-w/2, h/2);
    updateAll();
}

void DFCardItem::updateMagPoint()
{
    QRectF rect = sizeRect();
    (*mags)[0]->setPos({rect.left(), 0});
    (*mags)[1]->setPos({rect.right(), 0});
    (*mags)[2]->setPos({0, rect.top()});
    (*mags)[3]->setPos({0, rect.bottom()});
}

void DFCardItem::modiToPoint(QPointF p, int id)
{
    QRectF rc = sizeRect();
    qreal nx = p.x();
    qreal ny = p.y();
    switch(id)
    {
    case 0:
        ny = qMin(ny,(rc.top()+rc.bottom())/2 ); ny = qMax(ny, rc.top());
        polygon[0].setY(ny);
        updateModiPoint();
        updateMagPoint();
        break;
    case 1:
        nx = qMin(nx, (rc.left()+rc.right())/2); nx = qMax(nx, rc.left());
        polygon[1].setX(nx);
        updateModiPoint();
        updateMagPoint();
        break;
    }
    return;
}

void DFCardItem::updateModiPoint()
{
    modis[0] = polygon[0];
    modis[1] = polygon[1];
}

//==============================================================================

void DFCardItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
    DPolygonBase::serialize(out, fa);
}

bool DFCardItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
    if(!DPolygonBase::deserialize(in, fa)) return false;
    updateAll();
    return true;
}
