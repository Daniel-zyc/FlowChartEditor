#pragma once

#include "dshapebase.h"
#include <QRectF>

class DRectItem : public DShapeBase
{
public:
	enum { Type = UserTypes::DRectItemType };
	DRectItem(QGraphicsItem *parent = nullptr);
	DRectItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

public:
	int type() const override { return Type; }
	void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

	QRectF sizeRect() const override;
	QPainterPath shapeNormal() const override;
	void resizeToRect(QRectF nrect) override;
	void modifyToPoint(QPointF p, int id) override;

private:
	void updateMagPoint();

	QRectF rect;
};

