#pragma once

#include "dshapebase.h"
#include "magpoint.h"

#include <QPolygonF>

class DPolygonBase : public DShapeBase
{
public:
	enum { Type = DConst::DPolygonBaseType };
	DPolygonBase(QGraphicsItem *parent = nullptr);
	DPolygonBase(QPolygonF polygon, QGraphicsItem *parent = nullptr);

public:
	int type() const override { return Type; }
	void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

	QRectF sizeRect() const override;
	QPainterPath shapeNormal() const override;
	virtual void sizeToRect(QRectF nrect) override;
	virtual void modiToPoint(QPointF p, int id) override;

protected:
	QPolygonF polygon;
};
