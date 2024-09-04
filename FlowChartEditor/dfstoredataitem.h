#pragma once

#include "dshapebase.h"

class DFStoreDataItem : public DShapeBase
{
public:
    enum { Type = DFStoreDataItemType };
    DFStoreDataItem(QGraphicsItem *parent = nullptr);
    DFStoreDataItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

public:
    int type() const override { return Type; }

    void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF sizeRect() const override;
    QPainterPath shapeNormal() const override;
    void sizeToRect(QRectF nrect) override;
    void modiToPoint(QPointF p, int id) override;

    void updateAll();
    void updatePath();
    void updateMagPoint();

protected:
    QRectF rect = QRectF(0, 0, 0, 0);
    QPainterPath path;

    void serialize(QDataStream &out, const QGraphicsItem* fa) const override;
    bool deserialize(QDataStream &in, QGraphicsItem* fa) override;
};
