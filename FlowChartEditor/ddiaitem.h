#pragma once
#include "dshapebase.h"
#include <QPainter>
#include <QPolygonF>
#include <QRectF>

class DShapeBase;

class DDiaItem : public DShapeBase
{
public:
    enum { Type = DDiaItemType };

    DDiaItem(QGraphicsItem *parent = nullptr);
    DDiaItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

    int type() const override { return Type; }

    void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    QRectF sizeRect() const override;
    QPainterPath shapeNormal() const override;
    void sizeToRect(QRectF nrect) override;
    void modiToPoint(QPointF p, int id) override;

private:
    void setRect(const QRectF &nrect);
    void updateMagPoint();
 //   void updateModiPoint();

    QRectF rect = QRectF(0, 0, 0, 0);
};
