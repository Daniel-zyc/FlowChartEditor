#include "dpolygon.h"

DPolygon::DPolygon(QGraphicsItem *parent)
    :DShapeBase(parent)
{
}

DPolygon::DPolygon(QPolygonF polygon, QGraphicsItem *parent)
    :DPolygon(parent)
{
    this->polygon = polygon;
}

void DPolygon::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawPolygon(polygon);
}

QRectF DPolygon::sizeRect() const
{
    return polygon.boundingRect();
}

void DPolygon::resizeToRect(QRectF nrect)
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

QPainterPath DPolygon::shapeNormal() const
{
    QPainterPath pth;
    pth.addPolygon(polygon);
    return pth;
}
