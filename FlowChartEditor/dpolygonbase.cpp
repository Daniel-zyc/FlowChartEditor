#include "dpolygonbase.h"

DPolygonBase::DPolygonBase(QGraphicsItem *parent)
	: DShapeBase(parent) {}

void DPolygonBase::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

    // setBrush(QBrush(Qt::transparent));
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
	double x_ratio = nrect.width() / rect.width();
	double y_ratio = nrect.height() / rect.height();
	for(int i = 0; i < polygon.size(); i++)
		poly << QPointF(polygon.at(i).x() * x_ratio, polygon.at(i).y() * y_ratio);
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
