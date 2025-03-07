#pragma once

#include "../base/dshapebase.h"
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

	void updateAll();
	void updateMagPoint();
	void updateModiPoint();

protected:
	QRectF rect;
	qreal ratiox = 0.2, ratioy = 0.2;

public:
	// 序列化
	void serialize(QDataStream &out, const QGraphicsItem* fa) const override;
	bool deserialize(QDataStream &in, QGraphicsItem* fa) override;
};

