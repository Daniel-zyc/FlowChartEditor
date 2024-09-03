#pragma once

#include "global.h"
#include "dlinebase.h"

class DLineItem : public DLineBase
{
public:
	enum { Type = DLineItemType };
	DLineItem(QGraphicsItem *parent = nullptr);
	DLineItem(QPointF begin, QPointF end, QGraphicsItem *parent = nullptr);
	~DLineItem() = default;

public:
	int type() const override { return Type; }

	QRectF boundingRect() const override;
	void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	void modiToPoint(QPointF p, int id) override;
	QPainterPath shapeNormal() const override;

	void updateAll();
	void updateLine() override;
	void updatePath();

protected:
	QPainterPath path;

public:
	/**
	 * @brief serialize
	 * @param out
	 * 序列化：父类 DLineBase 序列化
	 */
	void serialize(QDataStream &out, const QGraphicsItem* fa = nullptr) const override;
	bool deserialize(QDataStream &in, QGraphicsItem* fa = nullptr) override;
};
