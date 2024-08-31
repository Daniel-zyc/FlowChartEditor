#include "dpolygonbase.h"

DPolygonBase::DPolygonBase(QGraphicsItem *parent)
	: DShapeBase(parent) {}

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
	QPolygonF poly;
	QRectF rect = sizeRect();
	double ratiox = nrect.width() / rect.width();
	double ratioy = nrect.height() / rect.height();
	for(int i = 0; i < polygon.size(); i++)
		poly << QPointF(polygon[i].x() * ratiox, polygon[i].y() * ratioy);
	setPoly(poly);
}

QPainterPath DPolygonBase::shapeNormal() const
{
	QPainterPath pth;
	pth.addPolygon(polygon);
	return pth;
}

void DPolygonBase::setPoly(const QPolygonF &npoly)
{
	polygon = npoly;
	sizeRectUpdated();
	updateMagPoint();
	updateModiPoint();
}

//==============================================
void DPolygonBase::serialize(QDataStream &out) const{
    DShapeBase::serialize(out);

    out << polygon;
}

void DPolygonBase::deserialize(QDataStream &in){
    DShapeBase::deserialize(in);

    in >> polygon;
}
