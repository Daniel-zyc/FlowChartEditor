#pragma once

#include "global.h"
#include "dlinebase.h"

class DShapeBase;

class DLineItem : public DLineBase
{
public:
	enum { Type = DLineItemType };
	DLineItem(QGraphicsItem *parent = nullptr);
	DLineItem(QPointF begin, QPointF end, QGraphicsItem *parent = nullptr);

	~DLineItem() = default;

	int type() const override { return Type; }

	QRectF boundingRect() const override;

protected:
	void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

	void modiToPoint(QPointF p, int id) override;

	QPainterPath shapeNormal() const override;

	void updateLine() override;

public:
    /**
     * @brief serialize
     * @param out
     * 序列化：父类DLineBase序列化
     */
    void serialize(QDataStream &out) const;

    void deserialize(QDataStream &in);
};
