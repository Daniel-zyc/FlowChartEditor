#pragma once

#include "../base/dshapebase.h"
#include <QRectF>

class DFPredefineItem : public DShapeBase
{
public:
	enum { Type = DFPredefineItemType };
	DFPredefineItem(QGraphicsItem *parent = nullptr);
	DFPredefineItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

public:
    int type() const override { return Type; }

    void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF sizeRect() const override;
    QPainterPath shapeNormal() const override;
    void sizeToRect(QRectF nrect) override;
    void modiToPoint(QPointF p, int id) override;

    void updateAll();
    void updateMagPoint();
    void updateModiPoint();

protected:
    QRectF rect = QRectF(0, 0, 0, 0);
    qreal ratio = 0.15;

public:
	void serialize(QDataStream &out, const QGraphicsItem* fa) const override;
	bool deserialize(QDataStream &in, QGraphicsItem* fa) override;
};
