#include "dellitem.h"
#include "magpoint.h"

DEllItem::DEllItem(QGraphicsItem *parent)
	: DShapeBase(parent) {}

DEllItem::DEllItem(qreal w, qreal h, QGraphicsItem *parent)
	: DEllItem(parent)
{
	mags->push_back(new MagPoint(this));
	mags->push_back(new MagPoint(this));
	mags->push_back(new MagPoint(this));
	mags->push_back(new MagPoint(this));
	setRect(QRectF(-w/2, -h/2, w, h));
}

void DEllItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	// setBrush(QBrush(Qt::transparent));
	painter->setBrush(brush());
	painter->setPen(pen());
	painter->drawEllipse(rect);
}

QRectF DEllItem::sizeRect() const
{
	return rect;
}

QPainterPath DEllItem::shapeNormal() const
{
	QPainterPath pth;
	pth.addEllipse(rect);
	return pth;
}

void DEllItem::updateMagPoint()
{
	(*mags)[0]->pos = {rect.left(), 0};
	(*mags)[1]->pos = {rect.right(), 0};

	(*mags)[2]->pos = {0, rect.top()};
	(*mags)[3]->pos = {0, rect.bottom()};
}

void DEllItem::sizeToRect(QRectF nrect)
{
	setRect(nrect);
}

void DEllItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(p); Q_UNUSED(id);
	return;
}

void DEllItem::setRect(const QRectF &nrect)
{
	rect = nrect;
	sizeRectUpdated();
	updateMagPoint();
}

//======================================

void DEllItem::serialize(QDataStream &out) const{
    DShapeBase::serialize(out);

    out << rect;
}

void DEllItem::deserialize(QDataStream &in){
    DShapeBase::deserialize(in);

    in >> rect;
}
