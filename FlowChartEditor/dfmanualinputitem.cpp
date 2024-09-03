#include "dfmanualinputitem.h"
#include "magpoint.h"

DFManualInputItem::DFManualInputItem(QGraphicsItem *parent)
    : DFManualInputItem(minRectSize, minRectSize, parent) {}

DFManualInputItem::DFManualInputItem(qreal w,qreal h, QGraphicsItem *parent)
    : DPolygonBase("", parent)
{
    modis.resize(1);
    for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
    polygon << QPointF(-w/2, -0.3*h) << QPointF(w/2, -h/2)
            << QPointF(w/2, h/2) << QPointF(-w/2, h/2);
    updateAll();
}

void DFManualInputItem::updateMagPoint()
{
    (*mags)[0]->setPos((polygon[0] + polygon[1]) / 2);
    (*mags)[1]->setPos((polygon[1] + polygon[2]) / 2);
    (*mags)[2]->setPos((polygon[2] + polygon[3]) / 2);
    (*mags)[3]->setPos((polygon[3] + polygon[0]) / 2);
}

void DFManualInputItem::modiToPoint(QPointF p, int id)
{
    Q_UNUSED(id);
    QRectF rc = sizeRect();
    qreal ny = p.y();
    ny = qMin(ny, rc.bottom()); ny = qMax(ny, rc.top());
    polygon[0].setY(ny);
    updateModiPoint();
    updateMagPoint();
}

void DFManualInputItem::updateModiPoint()
{
    modis[0] = polygon[0];
}
