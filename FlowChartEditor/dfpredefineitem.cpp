#include "dfpredefineitem.h"
#include "magpoint.h"

DPreItem::DPreItem(QGraphicsItem *parent)
    : DPreItem(minRectSize, minRectSize, parent) {}

DPreItem::DPreItem(qreal w, qreal h, QGraphicsItem *parent)
    : DShapeBase("", parent)
{
    modis.resize(1);
    for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
    rect = QRectF(-w/2, -h/2, w, h);
    updateAll();
}

void DPreItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    painter->setBrush(brush());
    painter->setPen(pen());
    painter->drawRect(rect);
    painter->drawLine(QPointF(rect.left()+ratio*rect.width(), -0.5*rect.height()), QPointF(rect.left()+ratio*rect.width(), 0.5*rect.height()));
    painter->drawLine(QPointF(rect.right()-ratio*rect.width(), -0.5*rect.height()), QPointF(rect.right()-ratio*rect.width(), 0.5*rect.height()));
}

QRectF DPreItem::sizeRect() const
{
    return rect;
}

QPainterPath DPreItem::shapeNormal() const
{
    QPainterPath pth;
    pth.addRect(rect);
    return pth;
}
void DPreItem::updateModiPoint()
{
    modis[0] = {rect.left() + rect.width() * ratio, rect.top()};
}

void DPreItem::updateMagPoint()
{
    (*mags)[0]->setPos({rect.left(), 0});
    (*mags)[1]->setPos({rect.right(), 0});

    (*mags)[2]->setPos({0, rect.top()});
    (*mags)[3]->setPos({0, rect.bottom()});
}

void DPreItem::sizeToRect(QRectF nrect)
{
    rect = nrect;
    updateAll();
}

void DPreItem::modiToPoint(QPointF p, int id)
{
    switch(id)
    {
    case 0:
        ratio = (p.x() - rect.left()) / rect.width();
        ratio = qMin(ratio, 0.5);
        ratio = qMax(0.0, ratio);
        updateModiPoint();
        break;
    }
}
void DPreItem::updateAll()
{
    updateSizePoint();
    updateMagPoint();
    updateModiPoint();
}
