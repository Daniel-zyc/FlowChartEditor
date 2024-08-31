#pragma once

#include "dshapebase.h"
#include <QRectF>

class DTrapItem : public DShapeBase
{
public:
    enum { Type = DTrapItemType };
    DTrapItem(QGraphicsItem *parent = nullptr);
    DTrapItem(qreal topWidth, qreal bottomWidth, qreal height, QGraphicsItem *parent = nullptr);

public:
    int type() const override { return Type; }
    void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    QRectF sizeRect() const override;
    QPainterPath shapeNormal() const override;
    void sizeToRect(QRectF nrect) override;
    void modiToPoint(QPointF p, int id) override;

private:
    void setRect(const QRectF &nrect);
    void updateMagPoint();
    void updateModiPoint();
    qreal topWidth;
    qreal bottomWidth;
    qreal height;
    qreal c;

    QRectF rect;

};
