#include "dmultidocitem.h"
#include "magpoint.h"

DMultiDocItem::DMultiDocItem(QGraphicsItem *parent)
    : DMultiDocItem(minRectSize, minRectSize, parent) {}

DMultiDocItem::DMultiDocItem(qreal w, qreal h, QGraphicsItem *parent)
    : DShapeBase("", parent)
{
    for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
    rect = QRectF(-w/2, -h/2, w, h);
    //updateAll();
}

void DMultiDocItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    painter->setBrush(brush());
    painter->setPen(pen());
    painter->drawPath(path);
}

QRectF DMultiDocItem::sizeRect() const
{
    return rect;
}

QPainterPath DMultiDocItem::shapeNormal() const
{
    return path;
}

void DMultiDocItem::sizeToRect(QRectF nrect)
{
    rect = nrect;
    //updateAll();
}

void DMultiDocItem::modiToPoint(QPointF p, int id)
{
    Q_UNUSED(p); Q_UNUSED(id);
    return;
}

