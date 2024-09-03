#pragma once

#include "dshapebase.h"
#include <QRectF>

class DShapeBase;

class DDocItem : public DShapeBase
{
public:
	enum { Type = DDocItemType };
	DDocItem(QGraphicsItem *parent = nullptr);
	DDocItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

public:
	int type() const override { return Type; }

	void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	QRectF sizeRect() const override;
	QPainterPath shapeNormal() const override;
	void sizeToRect(QRectF nrect) override;
	void modiToPoint(QPointF p, int id) override;

	void updateAll();
	void updatePath();
	void updateMagPoint();

protected:
	QRectF rect = QRectF(0, 0, 0, 0);
	QPainterPath path;

public:
	/**
	 * @brief serialize
	 * @param out
	 * 序列化：DShapeBase -> rect -> radiusx -> radiusy
	 */
	void serialize(QDataStream &out, const QGraphicsItem* fa) const override;
	bool deserialize(QDataStream &in, QGraphicsItem* fa) override;
};
