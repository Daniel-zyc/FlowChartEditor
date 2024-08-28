#include "dpolygonbase.h"

DPolygonBase::DPolygonBase(QGraphicsItem *parent)
    :DShapeBase(parent)
{
}

DPolygonBase::DPolygonBase(QPolygonF polygon, QGraphicsItem *parent)
    :DPolygonBase(parent)
{
    this->polygon = polygon;
}

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

void DPolygonBase::resizeToRect(QRectF nrect)
{
    QPolygonF poly;
    QRectF rect = sizeRect();
    double x_ratio = nrect.width() / rect.width();
    double y_ratio = nrect.height() / rect.height();
    for(int i = 0; i < polygon.size(); i++) {
        poly << QPointF(polygon.at(i).x() * x_ratio, polygon.at(i).y() * y_ratio);
    }
    polygon.swap(poly);
}

QPainterPath DPolygonBase::shapeNormal() const
{
    QPainterPath pth;
    pth.addPolygon(polygon);
    return pth;
}

void DPolygonBase::modifyToPoint(QPointF p, int id)
{
	Q_UNUSED(p); Q_UNUSED(id);
	return;
}
