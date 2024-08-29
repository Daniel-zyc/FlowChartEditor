#pragma once

#include "dpolygonbase.h"

class DTriangle : public DPolygonBase
{
public:
	enum { Type = DConst::DPolygonBaseType };
	DTriangle(QGraphicsItem *parent = nullptr);
	DTriangle(qreal w, qreal h, QGraphicsItem *parent = nullptr);

public:
	int type() const override { return Type; }
	virtual void modiToPoint(QPointF p, int id) override;

protected:
	void updateMagPoint() override;
	void updateModiPoint() override;
};

