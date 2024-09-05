#pragma once

#include "../base/dpolygonbase.h"

class DTriItem : public DPolygonBase
{
public:
	enum { Type = DTriItemType };
	DTriItem(QGraphicsItem *parent = nullptr);
	DTriItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

public:
	int type() const override { return Type; }

	void modiToPoint(QPointF p, int id) override;

	void updateMagPoint() override;
	void updateModiPoint() override;

public:
    /**
     * @brief serialize
     * @param out
     * 序列化：DPolygonBase
     */
	void serialize(QDataStream &out, const QGraphicsItem* fa) const override;
	bool deserialize(QDataStream &in, QGraphicsItem* fa) override;
};

