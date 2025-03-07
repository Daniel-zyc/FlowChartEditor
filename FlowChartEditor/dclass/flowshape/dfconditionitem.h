#pragma once

#include "../base/dpolygonbase.h"

class DFConditionItem : public DPolygonBase
{
public:
	enum { Type = DFConditionItemType };
	DFConditionItem(QGraphicsItem *parent = nullptr);
	DFConditionItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

public:
	int type() const override { return Type; }

	void modiToPoint(QPointF p, int id) override;

	void updateMagPoint() override;
	void updateModiPoint() override;

public:
	/**
	 * @brief serialize
	 * @param out
	 * 序列化：DShapeBase -> rect
	 */
	void serialize(QDataStream &out, const QGraphicsItem* fa) const override;
	bool deserialize(QDataStream &in, QGraphicsItem* fa) override;
};
