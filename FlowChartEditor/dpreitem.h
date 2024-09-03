#pragma once

#include "dshapebase.h"
#include <QRectF>

class DShapeBase;

class DPreItem : public DShapeBase
{
public:
    enum { Type = DPreItemType };
    DPreItem(QGraphicsItem *parent = nullptr);
    DPreItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

public:
    int type() const override { return Type; }

    void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF sizeRect() const override;
    QPainterPath shapeNormal() const override;
    void sizeToRect(QRectF nrect) override;
    void modiToPoint(QPointF p, int id) override;

    void updateAll();
    void updateMagPoint();

protected:
    QRectF rect = QRectF(0, 0, 0, 0);
    QRectF rect1 = QRectF(0, 0, 0, 0);

};
