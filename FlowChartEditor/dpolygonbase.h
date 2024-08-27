#ifndef DPOLYGONBASE_H
#define DPOLYGONBASE_H

#include "dshapebase.h"
#include <QPolygonF>

class DPolygonBase : public DShapeBase
{
public:
	enum { Type = UserTypes::DPolygonBaseType };
	DPolygonBase(QGraphicsItem *parent = nullptr);
	DPolygonBase(QPolygonF polygon, QGraphicsItem *parent = nullptr);

public:
	int type() const override { return Type; }
	void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

	QRectF sizeRect() const override;
	QPainterPath shapeNormal() const override;
	virtual void resizeToRect(QRectF nrect) override;
	virtual void modifyToPoint(QPointF p, int id) override;

protected:
	QPolygonF polygon;
};


#endif // DPOLYGONBASE_H
