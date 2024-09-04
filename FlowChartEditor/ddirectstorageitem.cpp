#include "ddirectstorageitem.h"
#include "magpoint.h"

DDirectStorageItem::DDirectStorageItem(QGraphicsItem *parent)
    : DDirectStorageItem(minRectSize, minRectSize, parent) {}

DDirectStorageItem::DDirectStorageItem(qreal w, qreal h, QGraphicsItem *parent)
    : DShapeBase("", parent)
{
    for(int i = 0; i < 5; i++) mags->push_back(new MagPoint(this));
    rect = QRectF(-w/2, -h/2, w, h);
    updateAll();
}

void DDirectStorageItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    painter->setBrush(brush());
    painter->setPen(pen());
    painter->drawPath(path);
}

QRectF DDirectStorageItem::sizeRect() const
{
    return rect;
}

QPainterPath DDirectStorageItem::shapeNormal() const
{
    return path;
}

void DDirectStorageItem::updateMagPoint()
{
    (*mags)[0]->setPos({rect.left(), 0});
    (*mags)[1]->setPos({rect.right(), 0});
    (*mags)[2]->setPos({0, rect.top()});
    (*mags)[3]->setPos({0, rect.bottom()});
    (*mags)[4]->setPos({rect.right() / 3, 0});
}

void DDirectStorageItem::updatePath()
{
    path.clear();

    path.moveTo(rect.right() * 2 / 3, rect.top());
    path.arcTo(rect.right() / 3, rect.top(), rect.width() / 3, rect.height(), 90, -180);
    path.lineTo(rect.left() * 2 / 3, rect.bottom());
    path.arcTo(rect.left(), rect.top(), rect.width() / 3, rect.height(), -90, -180);
    path.lineTo(rect.right() * 2 / 3, rect.top());
    path.arcTo(rect.right() / 3, rect.top(), rect.width() / 3, rect.height(), 90, 180);
}

void DDirectStorageItem::sizeToRect(QRectF nrect)
{
    rect = nrect; updateAll();
}

void DDirectStorageItem::modiToPoint(QPointF p, int id)
{
    Q_UNUSED(p); Q_UNUSED(id); return;
}


void DDirectStorageItem::updateAll()
{
    updateSizePoint();
    updatePath();
    updateMagPoint();
}

//==============================================================================

void DDirectStorageItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
    DShapeBase::serialize(out, fa);

    out << rect;
}

bool DDirectStorageItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
    if(!DShapeBase::deserialize(in, fa)) return false;

    in >> rect;
    updateAll();
    return true;
}
