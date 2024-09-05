#include "dfshowitem.h"
#include "../../magpoint.h"

DFShowItem::DFShowItem(QGraphicsItem *parent)
	: DFShowItem(minRectSize, minRectSize, parent) {}

DFShowItem::DFShowItem(qreal w, qreal h, QGraphicsItem *parent)
    : DShapeBase("", parent)
{
    for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
    rect = QRectF(-w/2, -h/2, w, h);
    updateAll();
}

void DFShowItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    painter->setBrush(brush());
    painter->setPen(pen());
    painter->drawPath(path);
}

QRectF DFShowItem::sizeRect() const
{
    return rect;
}

QPainterPath DFShowItem::shapeNormal() const
{
    return path;
}

void DFShowItem::updateMagPoint()
{
    (*mags)[0]->setPos({rect.left(), 0});
    (*mags)[1]->setPos({rect.right(), 0});
    (*mags)[2]->setPos({0, rect.top()});
    (*mags)[3]->setPos({0, rect.bottom()});
}

void DFShowItem::updatePath()
{
    path.clear();

    path.moveTo(rect.topLeft() + QPointF(rect.width() / 6, 0));
    path.lineTo(rect.topRight() - QPointF(rect.width() / 6, 0));
    path.arcTo(rect.width() / 6, -rect.height() / 2, rect.width() / 3, rect.height(), 90, -180);
    path.lineTo(rect.bottomLeft() + QPointF(rect.width() / 6, 0));
    path.lineTo(-rect.width() / 2, 0);

    path.closeSubpath();
}

void DFShowItem::sizeToRect(QRectF nrect)
{
    rect = nrect; updateAll();
}

void DFShowItem::modiToPoint(QPointF p, int id)
{
    Q_UNUSED(p); Q_UNUSED(id); return;
}

void DFShowItem::updateAll()
{
    updateSizePoint();
    updatePath();
    updateMagPoint();
}

//==============================================================================

void DFShowItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
    DShapeBase::serialize(out, fa);

    out << rect;
}

bool DFShowItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
    if(!DShapeBase::deserialize(in, fa)) return false;

    in >> rect;
    updateAll();
    return true;
}
