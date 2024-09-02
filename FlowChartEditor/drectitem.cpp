#include "drectitem.h"
#include "magpoint.h"

DRectItem::DRectItem(QGraphicsItem *parent)
	: DRectItem(minRectSize, minRectSize, parent) {}

DRectItem::DRectItem(qreal w, qreal h, QGraphicsItem *parent)
	: DShapeBase("", parent)
{
	for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
	rect = QRectF(-w/2, -h/2, w, h);
	updateAll();
}

void DRectItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	painter->setBrush(brush());
	painter->setPen(pen());
	painter->drawRect(rect);
}

QRectF DRectItem::sizeRect() const
{
	return rect;
}

QPainterPath DRectItem::shapeNormal() const
{
	QPainterPath pth; pth.addRect(rect); return pth;
}

void DRectItem::updateMagPoint()
{
	(*mags)[0]->setPos({rect.left(), 0});
	(*mags)[1]->setPos({rect.right(), 0});

	(*mags)[2]->setPos({0, rect.top()});
	(*mags)[3]->setPos({0, rect.bottom()});
}

void DRectItem::sizeToRect(QRectF nrect)
{
	rect = nrect; updateAll();
}

void DRectItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(p); Q_UNUSED(id); return;
}

void DRectItem::updateAll()
{
	updateSizePoint();
	updateMagPoint();
}

//==============================================================================

void DRectItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DShapeBase::serialize(out, fa);

	out << rect;
}

bool DRectItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DShapeBase::deserialize(in, fa)) return false;

	in >> rect;
	updateAll();
	return true;
}
