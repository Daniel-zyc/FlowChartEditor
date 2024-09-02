#include "denditem.h"
#include "magpoint.h"

DEndItem::DEndItem(QGraphicsItem *parent)
    : DEndItem(minRectSize, minRectSize, parent) {}

DEndItem::DEndItem(qreal w, qreal h, QGraphicsItem *parent)
    : DShapeBase(parent)
{
    for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
    rect = QRectF(-w/2, -h/2, w, h);
    updateAll();
}

void DEndItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    painter->setBrush(brush());
    painter->setPen(pen());
    painter->drawRoundedRect(rect, rect.width() / 5, rect.height() / 2);
}

QRectF DEndItem::sizeRect() const
{
    return rect;
}

QPainterPath DEndItem::shapeNormal() const
{
    QPainterPath pth;
    pth.addRoundedRect(rect, rect.width() / 5, rect.height() / 2);
    return pth;
}

void DEndItem::updateMagPoint()
{
    (*mags)[0]->pos = {rect.left(), 0};
    (*mags)[1]->pos = {rect.right(), 0};

    (*mags)[2]->pos = {0, rect.top()};
    (*mags)[3]->pos = {0, rect.bottom()};
}

void DEndItem::sizeToRect(QRectF nrect)
{
    rect = nrect; updateAll();
}

void DEndItem::modiToPoint(QPointF p, int id)
{
    Q_UNUSED(p); Q_UNUSED(id);
    return;
}

void DEndItem::updateAll()
{
    updateSizePoint();
    updateMagPoint();
}
