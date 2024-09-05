#pragma once

#include "dshapebase.h"

#include <QPolygonF>

class DPolygonBase : public DShapeBase
{
public:
	enum { Type = DPolygonBaseType };
	DPolygonBase(QGraphicsItem *parent = nullptr);
	DPolygonBase(const QString& text, QGraphicsItem *parent = nullptr);

public:
	virtual int type() const override { return Type; }

	virtual void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	virtual QRectF sizeRect() const override;
	virtual QPainterPath shapeNormal() const override;
	virtual void sizeToRect(QRectF nrect) override;

	virtual void updateAll();
	virtual void updateMagPoint() = 0;
	virtual void updateModiPoint() = 0;

protected:
	QPolygonF polygon;

public:
    /**
     * @brief serialize
     * @param out
     * 序列化：DShapeBase -> polygon
     */
	void serialize(QDataStream &out, const QGraphicsItem* fa) const override;
	bool deserialize(QDataStream &in, QGraphicsItem* fa) override;
};
