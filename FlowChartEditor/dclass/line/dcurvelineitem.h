#pragma once

#include "../../global.h"
#include "../base/dlinebase.h"

class DShapeBase;

class DCurveLineItem : public DLineBase
{
public:
	enum { Type = DCurveLineItemType };
	DCurveLineItem(QGraphicsItem *parent = nullptr);
	DCurveLineItem(QPointF begin, QPointF end, QGraphicsItem *parent = nullptr);
	~DCurveLineItem() = default;

public:
	int type() const override { return Type; }

	QRectF boundingRect() const override;
	void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	void modiToPoint(QPointF p, int id) override;
	QPainterPath shapeNormal() const override;
	void updateLine() override;

	void updateAll();
	void updateModiPoint();
	void updatePath();

private:
	QPointF tip;
	qreal angle;
	QPainterPath path;

public:
	/**
	 * @brief serialize
	 * @param out
	 * 序列化：父类DLineBase序列化
	 */
	void serialize(QDataStream &out, const QGraphicsItem* fa = nullptr) const override;
	bool deserialize(QDataStream &in, QGraphicsItem* fa = nullptr) override;
};
