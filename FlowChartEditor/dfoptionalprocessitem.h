#pragma once

#include "dshapebase.h"
#include <QRectF>

class DFOptionalProcessItem : public DShapeBase
{
public:
	enum { Type = DFOptionalProcessItemType };
	DFOptionalProcessItem(QGraphicsItem *parent = nullptr);
	DFOptionalProcessItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

public:
	int type() const override { return Type; }

	void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	QRectF sizeRect() const override;
	QPainterPath shapeNormal() const override;
	void sizeToRect(QRectF nrect) override;
	void modiToPoint(QPointF p, int id) override;

	void updateAll();
	void updateMagPoint();
	void updateModiPoint();

protected:
	QRectF rect;
	qreal ratiox = 0.2, ratioy = 0.2;

public:
	/**
	 * @brief serialize
	 * @param out
	 * 序列化：DShapeBase -> rect -> radiusx -> radiusy
	 */
	void serialize(QDataStream &out, const QGraphicsItem* fa) const override;
	bool deserialize(QDataStream &in, QGraphicsItem* fa) override;
};

