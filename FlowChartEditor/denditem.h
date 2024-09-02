#pragma once

#include "dshapebase.h"
#include <QRectF>
class DEndItem : public DShapeBase
{
public:
    enum { Type = DEndItemType };
    DEndItem(QGraphicsItem *parent = nullptr);
    DEndItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

public:
    int type() const override { return Type; }
    void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    QRectF sizeRect() const override;
    QPainterPath shapeNormal() const override;
    void sizeToRect(QRectF nrect) override;
    void modiToPoint(QPointF p, int id) override;

    void updateAll();
    void updateMagPoint();

private:
    void setRect(const QRectF &nrect);
    QRectF rect;
    qreal radius;

};
