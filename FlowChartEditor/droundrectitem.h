#pragma once

#include "dshapebase.h"
#include <QRectF>

class DRoundRectItem : public DShapeBase
{
public:
	enum { Type = DRoundRectItemType };
	DRoundRectItem(QGraphicsItem *parent = nullptr);
	DRoundRectItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

public:
	int type() const override { return Type; }
	void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

	QRectF sizeRect() const override;
	QPainterPath shapeNormal() const override;
	void sizeToRect(QRectF nrect) override;
	void modiToPoint(QPointF p, int id) override;

private:
	void setRect(const QRectF &nrect);
	void updateMagPoint();
	void updateModiPoint();

	QRectF rect;
	qreal radiusx = 30, radiusy = 30;

public:
	/**
	 * @brief serialize
	 * @param out
	 * 序列化：DShapeBase -> rect -> radiusx -> radiusy
	 */
	void serialize(QDataStream &out, const QGraphicsItem* fa) const override;

	bool deserialize(QDataStream &in, QGraphicsItem* fa) override;
};

