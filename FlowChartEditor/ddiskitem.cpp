#include "ddiskitem.h"
#include "magpoint.h"

DDiskItem::DDiskItem(QGraphicsItem *parent)
    : DDiskItem(minRectSize, minRectSize, parent) {}

DDiskItem::DDiskItem(qreal w, qreal h, QGraphicsItem *parent)
    : DShapeBase("", parent)
{
    for(int i = 0; i < 5; i++) mags->push_back(new MagPoint(this));
    rect = QRectF(-w/2, -h/2, w, h);
    updateAll();
}

void DDiskItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    painter->setBrush(brush());
    painter->setPen(pen());
    painter->drawPath(path);
}

QRectF DDiskItem::sizeRect() const
{
    return rect;
}

QPainterPath DDiskItem::shapeNormal() const
{
    return path;
}

void DDiskItem::updateMagPoint()
{
    (*mags)[0]->setPos({rect.left(), 0});
    (*mags)[1]->setPos({rect.right(), 0});
    (*mags)[2]->setPos({0, rect.top()});
    (*mags)[3]->setPos({0, rect.bottom()});
    (*mags)[4]->setPos({0, rect.top() / 3});
}

void DDiskItem::updatePath()
{
    path.clear();

    path.moveTo(rect.left(), rect.top() * 2 / 3);
    path.arcTo(rect.left(), rect.top(), rect.width(), rect.height() / 3, 180, -180);
    path.lineTo(rect.right(), rect.bottom() * 2 / 3);
    path.arcTo(rect.left(), rect.bottom() / 3, rect.width(), rect.height() / 3, 0, -180);
    path.lineTo(rect.left(), rect.top() * 2 / 3);
    path.arcTo(rect.left(), rect.top(), rect.width(), rect.height() / 3, 180, 180);
}

void DDiskItem::sizeToRect(QRectF nrect)
{
    rect = nrect; updateAll();
}

void DDiskItem::modiToPoint(QPointF p, int id)
{
    Q_UNUSED(p); Q_UNUSED(id); return;
}

void DDiskItem::updateAll()
{
    updateSizePoint();
    updatePath();
    updateMagPoint();
}

//==============================================================================

void DDiskItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
    DShapeBase::serialize(out, fa);

    out << rect;
}

bool DDiskItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
    if(!DShapeBase::deserialize(in, fa)) return false;

    in >> rect;
    updateAll();
    return true;
}
