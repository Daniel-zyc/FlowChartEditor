#include "dfstartenditem.h"
#include "../../magpoint.h"

DFStartEndItem::DFStartEndItem(QGraphicsItem *parent)
    : DFStartEndItem(minRectSize, minRectSize, parent) {}

DFStartEndItem::DFStartEndItem(qreal w, qreal h, QGraphicsItem *parent)
	: DShapeBase("", parent)
{
    for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
    rect = QRectF(-w/2, -h/2, w, h);
    updateAll();
}

void DFStartEndItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    painter->setBrush(brush());
    painter->setPen(pen());
    painter->drawRoundedRect(rect, rect.width() / 5, rect.height() / 2);
}

QRectF DFStartEndItem::sizeRect() const
{
    return rect;
}

QPainterPath DFStartEndItem::shapeNormal() const
{
	QPainterPath pth; pth.addRoundedRect(rect, rect.width() / 5, rect.height() / 2); return pth;
}

void DFStartEndItem::updateMagPoint()
{
	(*mags)[0]->pos = {rect.left(), 0};
	(*mags)[1]->pos = {rect.right(), 0};

	(*mags)[2]->pos = {0, rect.top()};
	(*mags)[3]->pos = {0, rect.bottom()};
}

void DFStartEndItem::sizeToRect(QRectF nrect)
{
	rect = nrect; updateAll();
}

void DFStartEndItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(p); Q_UNUSED(id); return;
}

void DFStartEndItem::updateAll()
{
    updateSizePoint();
    updateMagPoint();
}

void DFStartEndItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DShapeBase::serialize(out, fa);

	out << rect;
}

bool DFStartEndItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DShapeBase::deserialize(in, fa)) return false;

	in >> rect;
	updateAll();
	return true;
}
