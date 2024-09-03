#include "dpreitem.h"
#include "magpoint.h"

DPreItem::DPreItem(QGraphicsItem *parent)
    : DPreItem(minRectSize, minRectSize, parent) {}

DPreItem::DPreItem(qreal w, qreal h, QGraphicsItem *parent)
    : DShapeBase("", parent)
{
    for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
    rect1 = QRectF(-w/2+0.15*w, -h/2, 0.7*w, h);
    rect = QRectF(-w/2, -h/2, w, h);
    updateAll();
}

void DPreItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    painter->setBrush(brush());
    painter->setPen(pen());
    painter->drawRect(rect);
    painter->drawRect(rect1);
}

QRectF DPreItem::sizeRect() const
{
    return rect;
}

QPainterPath DPreItem::shapeNormal() const
{
    QPainterPath pth;
    pth.addRect(rect);
    pth.addRect(rect1);
    return pth;
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
    rect1 = QRectF(rect.left() + 0.15 * rect.width(), rect.top(), 0.7 * rect.width(), rect.height());
    updateAll();
}

void DPreItem::modiToPoint(QPointF p, int id)
{
    Q_UNUSED(p); Q_UNUSED(id); return;
}

void DPreItem::updateAll()
{
    updateSizePoint();
    updateMagPoint();
}
