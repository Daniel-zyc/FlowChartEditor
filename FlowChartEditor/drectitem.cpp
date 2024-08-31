#include "drectitem.h"
#include "magpoint.h"

DRectItem::DRectItem(QGraphicsItem *parent)
	: DRectItem(minRectSize, minRectSize, parent) {}

DRectItem::DRectItem(qreal w, qreal h, QGraphicsItem *parent)
	: DShapeBase("", parent)
{
	for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
	setRect(QRectF(-w/2, -h/2, w, h));
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
	QPainterPath pth;
	pth.addRect(rect);
	return pth;
}

void DRectItem::updateMagPoint()
{
	(*mags)[0]->pos = {rect.left(), 0};
	(*mags)[1]->pos = {rect.right(), 0};

	(*mags)[2]->pos = {0, rect.top()};
	(*mags)[3]->pos = {0, rect.bottom()};
}

void DRectItem::sizeToRect(QRectF nrect)
{
	setRect(nrect);
}

void DRectItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(p); Q_UNUSED(id);
	return;
}

void DRectItem::setRect(const QRectF &nrect)
{
	rect = nrect;
	sizeRectUpdated();
	updateMagPoint();
}

//========================================

void DRectItem::serialize(QDataStream &out) const{
    qDebug() << "DRectItem serializing";
    DShapeBase::serialize(out);

    out << rect;
}

void DRectItem::deserialize(QDataStream &in){
    qDebug() << "DRectItem deserializing";
    DShapeBase::deserialize(in);

    in >> rect;
}
