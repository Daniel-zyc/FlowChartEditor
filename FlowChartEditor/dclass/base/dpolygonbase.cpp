#include "dpolygonbase.h"

DPolygonBase::DPolygonBase(QGraphicsItem *parent)
	: DPolygonBase("", parent) {}

DPolygonBase::DPolygonBase(const QString& text, QGraphicsItem* parent)
	: DShapeBase(text, parent) {}

void DPolygonBase::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	painter->setPen(pen());
	painter->setBrush(brush());
	painter->drawPolygon(polygon);
}

QRectF DPolygonBase::sizeRect() const
{
	return polygon.boundingRect();
}

void DPolygonBase::sizeToRect(QRectF nrect)
{
	QPolygonF poly; QRectF rect = sizeRect();
	qreal ratiox = nrect.width() / rect.width();
	qreal ratioy = nrect.height() / rect.height();
	for(int i = 0; i < polygon.size(); i++)
		poly << QPointF(polygon[i].x() * ratiox, polygon[i].y() * ratioy);
	polygon = poly; updateAll();
}

QPainterPath DPolygonBase::shapeNormal() const
{
	QPainterPath pth; pth.addPolygon(polygon); return pth;
}

void DPolygonBase::updateAll()
{
	updateSizePoint();
	updateMagPoint();
	updateModiPoint();
}

//==============================================================================

void DPolygonBase::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DShapeBase::serialize(out, fa);
	out << polygon;
}

bool DPolygonBase::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DShapeBase::deserialize(in, fa)) return false;
	in >> polygon;
	return true;
}
