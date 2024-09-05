#include "dforderstorageitem.h"
#include "../../magpoint.h"

DFOrderStorageItem::DFOrderStorageItem(QGraphicsItem *parent)
	: DFOrderStorageItem(minRectSize, minRectSize, parent) {}

DFOrderStorageItem::DFOrderStorageItem(qreal w, qreal h, QGraphicsItem *parent)
    : DShapeBase("", parent)
{
    for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
    rect = QRectF(-w/2, -h/2, w, h);
    updateAll();
}

void DFOrderStorageItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    painter->setBrush(brush());
    painter->setPen(pen());
    painter->drawPath(path);
}

QRectF DFOrderStorageItem::sizeRect() const
{
    return rect;
}

QPainterPath DFOrderStorageItem::shapeNormal() const
{
    return path;
}

void DFOrderStorageItem::updateMagPoint()
{
    (*mags)[0]->setPos({rect.left(), 0});
    (*mags)[1]->setPos({rect.right(), 0});
    (*mags)[2]->setPos({0, rect.top()});
    (*mags)[3]->setPos({0, rect.bottom()});
}

void DFOrderStorageItem::updatePath()
{
    path.clear();

    path.moveTo(0, rect.height() / 2);
    path.arcTo(-rect.width() / 2, -rect.height() / 2, rect.width(), rect.height(), 270, -315);

    path.lineTo(rect.width() / 2, rect.height() / (2 * sqrt(2)));
    path.lineTo(rect.width() / 2, rect.height() / 2);

    path.closeSubpath();
}

void DFOrderStorageItem::sizeToRect(QRectF nrect)
{
    rect = nrect; updateAll();
}

void DFOrderStorageItem::modiToPoint(QPointF p, int id)
{
    Q_UNUSED(p); Q_UNUSED(id); return;
}


void DFOrderStorageItem::updateAll()
{
    updateSizePoint();
    updatePath();
    updateMagPoint();
}

//==============================================================================

void DFOrderStorageItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
    DShapeBase::serialize(out, fa);

    out << rect;
}

bool DFOrderStorageItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
    if(!DShapeBase::deserialize(in, fa)) return false;

    in >> rect;
    updateAll();
    return true;
}
