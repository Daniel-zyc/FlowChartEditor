#pragma once

#include "global.h"
#include "dlinebase.h"

class DShapeBase;

class DCurveItem : public DLineBase
{
public:
    enum { Type = DCurveLineItemType };
    DCurveItem(QGraphicsItem *parent = nullptr);
    DCurveItem(QPointF begin, QPointF end, QGraphicsItem *parent = nullptr);

    ~DCurveItem() = default;

    int type() const override { return Type; }

    QRectF boundingRect() const override;

protected:
    void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void modiToPoint(QPointF p, int id) override;

    QPainterPath shapeNormal() const override;

    void updateLine() override;

private:
    QPointF tip;
    double angle;

    void updateModiPoint();
    QPainterPath drawCurve() const;

public:
    /**
     * @brief serialize
     * @param out
     * 序列化：父类DLineBase序列化
     */
    // void serialize(QDataStream &out) const override;

    // void deserialize(QDataStream &in) override;
};
