#pragma once

#include "dshapebase.h"
#include <QRectF>

class DShapeBase;

class DParallegramItem : public DShapeBase
{
public:
    enum { Type = DParallegramItemType };
    DParallegramItem(QGraphicsItem *parent = nullptr);
    DParallegramItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

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
    QPainterPath addParallelogram(const QRectF &rect, qreal skew) const;

    QRectF rect;
    qreal skew;
};

