#pragma once

#include "dpolygonbase.h"

class DFManualInputItem : public DPolygonBase
{
public:
	enum { Type = DFManualInputItemType };
	DFManualInputItem(QGraphicsItem *parent = nullptr);
	DFManualInputItem(qreal w, qreal h, QGraphicsItem* parent = nullptr);

public:
	int type() const override { return Type; }

	void modiToPoint(QPointF, int id) override;

	void updateMagPoint() override;
	void updateModiPoint() override;

public:
	void serialize(QDataStream &out, const QGraphicsItem* fa) const override;
	bool deserialize(QDataStream &in, QGraphicsItem* fa) override;
};

