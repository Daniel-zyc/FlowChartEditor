#pragma once

#include "dshapebase.h"

class DFStartEndItem : public DShapeBase
{
public:
	enum { Type = DFStartEndItemType };
	DFStartEndItem(QGraphicsItem *parent = nullptr);
	DFStartEndItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

public:
	int type() const override { return Type; }

	void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	QRectF sizeRect() const override;
	QPainterPath shapeNormal() const override;
	void sizeToRect(QRectF nrect) override;
	void modiToPoint(QPointF p, int id) override;

	void updateAll();
	void updateMagPoint();

private:
	QRectF rect;

public:
	/**
	 * @brief serialize
	 * @param out
	 * 序列化：DShapeBase -> rect
	 */
	void serialize(QDataStream &out, const QGraphicsItem* fa) const override;
	bool deserialize(QDataStream &in, QGraphicsItem* fa) override;
};
