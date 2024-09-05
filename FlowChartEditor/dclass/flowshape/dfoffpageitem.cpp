#include "dfoffpageitem.h"
#include "../../magpoint.h"

DFOffPageItem::DFOffPageItem(QGraphicsItem *parent)
    : DFOffPageItem(minRectSize, minRectSize, parent) {}

DFOffPageItem::DFOffPageItem(qreal w,qreal h, QGraphicsItem *parent)
    : DPolygonBase("", parent)
{
    modis.resize(1);
    for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
    polygon << QPointF(-w/2, h/4) << QPointF(-w/2, -h/2) << QPointF(w/2, -h/2)
            << QPointF(w/2, h/4)<< QPointF(0, h/2);
    updateAll();
}

void DFOffPageItem::updateMagPoint()
{
    QRectF rect = sizeRect();
    (*mags)[0]->setPos({rect.left(), 0});
    (*mags)[1]->setPos({rect.right(), 0});

    (*mags)[2]->setPos({0, rect.top()});
    (*mags)[3]->setPos({0, rect.bottom()});
}

void DFOffPageItem::modiToPoint(QPointF p, int id)
{
    Q_UNUSED(id);
    QRectF rc = sizeRect(); qreal ny = p.y();
    ny = qMin(ny, rc.bottom()); ny = qMax(ny, 0.0);
    polygon[0].setY(ny);
    polygon[3].setY(ny);
    updateModiPoint();
    updateMagPoint();
}

void DFOffPageItem::updateModiPoint()
{
    modis[0] = polygon[0];
}

//==============================================================================

void DFOffPageItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
    DPolygonBase::serialize(out, fa);
}

bool DFOffPageItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
    if(!DPolygonBase::deserialize(in, fa)) return false;
    updateAll();
    return true;
}
