#ifndef DPOLYGON_H
#define DPOLYGON_H

#include "dshapebase.h"
#include <QPolygonF>

class DPolygon : public DShapeBase
{
public:
    DPolygon(QGraphicsItem *parent = nullptr);
    DPolygon(QPolygonF polygon, QGraphicsItem *parent = nullptr);

    void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

    QRectF sizeRect() const;
    void resizeToRect(QRectF nrect);

    QPainterPath shapeNormal() const;

private:
    QPolygonF polygon;
};


#endif // DPOLYGON_H
