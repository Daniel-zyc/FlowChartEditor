#pragma once

#include "dpolygonbase.h"

class DFManualOperateItem : public DPolygonBase
{
public:
	enum { Type = DFManualOperateItemType };
	DFManualOperateItem(QGraphicsItem *parent = nullptr);
	DFManualOperateItem(qreal w, qreal h, QGraphicsItem* parent = nullptr);

public:
	int type() const override { return Type; }

	void modiToPoint(QPointF, int id) override;

	void updateMagPoint() override;
	void updateModiPoint() override;

public:
	/**
	 * @brief serialize
	 * @param out
	 * 序列化：DShapeBase -> topWidth -> bottomWidth -> height -> c -> rect
	 */
	void serialize(QDataStream &out, const QGraphicsItem* fa) const override;
	bool deserialize(QDataStream &in, QGraphicsItem* fa) override;
};

