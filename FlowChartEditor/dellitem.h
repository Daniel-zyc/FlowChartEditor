#pragma once

#include "dshapebase.h"
#include <QRectF>

class DEllItem : public DShapeBase
{
public:
	DEllItem(QGraphicsItem *parent = nullptr);
	DEllItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

	void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

	QRectF sizeRect() const;
	void resizeToRect(QRectF nrect) { rect = nrect; }

	QPainterPath shapeNormal() const;

private:
	QRectF rect;
};

