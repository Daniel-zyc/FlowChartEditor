#pragma once

#include "../base/dpolygonbase.h"

// 六边形
class DHexgonItem : public DPolygonBase
{
public:
	enum { Type = DHexgonItemType };
	DHexgonItem(QGraphicsItem *parent = nullptr);
	DHexgonItem(qreal w, qreal h, QGraphicsItem* parent = nullptr);

public:
	int type() const override { return Type; }

	void modiToPoint(QPointF, int id) override;

	void updateMagPoint() override;
	void updateModiPoint() override;

public:
	// 序列化
	void serialize(QDataStream &out, const QGraphicsItem* fa) const override;
	bool deserialize(QDataStream &in, QGraphicsItem* fa) override;
};
