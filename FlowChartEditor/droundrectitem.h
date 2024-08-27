#pragma once

#include "dshapebase.h"
#include <QRectF>

class DRoundRectItem : public DShapeBase
{
public:
	enum { Type = UserTypes::DRoundRectItemType };
	DRoundRectItem(QGraphicsItem *parent = nullptr);
	DRoundRectItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

public:
	int type() const override { return Type; }
	void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

	QRectF sizeRect() const override;
	QPainterPath shapeNormal() const override;
	void resizeToRect(QRectF nrect) override;
	void modifyToPoint(QPointF p, int id) override;

private:
	QRectF rect = QRectF(0, 0, 0, 0);
	qreal radiusx = 0, radiusy = 0;
};

