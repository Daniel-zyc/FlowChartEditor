#pragma once

#include "dshapebase.h"
#include <QRectF>

class DRectItem : public DShapeBase
{
public:
	DRectItem(QGraphicsItem *parent = nullptr);
	DRectItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

	void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

	QRectF sizeRect() const;
	void resizeToRect(QRectF nrect) { rect = nrect; }
private:
	QRectF rect;
};

