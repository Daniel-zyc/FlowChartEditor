#pragma once

#include "../base/dshapebase.h"

// 椭圆
class DEllItem : public DShapeBase
{
public:
	enum { Type = DEllItemType };
	DEllItem(QGraphicsItem *parent = nullptr);
	DEllItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

public:
	int type() const override { return Type; }

	void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	QRectF sizeRect() const override;
	QPainterPath shapeNormal() const override;
	void sizeToRect(QRectF nrect) override;
	void modiToPoint(QPointF p, int id) override;

	void updateAll();
	void updateMagPoint();

protected:
	QRectF rect = QRectF(0, 0, 0, 0);

public:
	/**
	 * @brief serialize
	 * @param out
	 * 序列化：DShapeBase -> rect
	 */
	void serialize(QDataStream &out, const QGraphicsItem* fa) const override;
	bool deserialize(QDataStream &in, QGraphicsItem* fa) override;
};
