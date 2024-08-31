#pragma once

#include "dshapebase.h"
#include <QRectF>

class DShapeBase;

class DDocItem : public DShapeBase
{
public:
    enum { Type = DDocItemType };
    DDocItem(QGraphicsItem *parent = nullptr);
    DDocItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

public:
    int type() const override { return Type; }
    void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    QRectF sizeRect() const override;
    QPainterPath shapeNormal() const override;
    void sizeToRect(QRectF nrect) override;
    void modiToPoint(QPointF p, int id) override;
	void setRect(const QRectF &nrect);

private:
    void updateMagPoint();
    QPainterPath addDocShape(const QRectF &rec) const;

    QRectF rect = QRectF(0, 0, 0, 0);
};
