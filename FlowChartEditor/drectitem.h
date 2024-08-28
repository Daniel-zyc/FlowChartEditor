#pragma once

#include "dshapebase.h"
#include <QRectF>

class DShapeBase;

class DRectItem : public DShapeBase
{
public:
	enum { Type = DConst::DRectItemType };
	DRectItem(QGraphicsItem *parent = nullptr);
	DRectItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

public:
	int type() const override { return Type; }
	void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

	QRectF sizeRect() const override;
	QPainterPath shapeNormal() const override;
	void sizeToRect(QRectF nrect) override;
	void modiToPoint(QPointF p, int id) override;

	void setRect(const QRectF &nrect);

private:
	void updateMagPoint();

	QRectF rect = QRectF(0, 0, 0, 0);
};

