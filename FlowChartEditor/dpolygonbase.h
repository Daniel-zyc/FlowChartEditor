#pragma once

#include "dshapebase.h"

#include <QPolygonF>

class DPolygonBase : public DShapeBase
{
public:
	enum { Type = DPolygonBaseType };
	DPolygonBase(QGraphicsItem *parent = nullptr);

public:
	virtual int type() const override { return Type; }

	void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	QRectF sizeRect() const override;
	QPainterPath shapeNormal() const override;
	virtual void sizeToRect(QRectF nrect) override;
	void setPoly(const QPolygonF &npoly);

protected:
	virtual void updateMagPoint() = 0;
	virtual void updateModiPoint() = 0;

	QPolygonF polygon;

public:
    /**
     * @brief serialize
     * @param out
     * 序列化：DShapeBase -> polygon
     */
    void serialize(QDataStream &out) const override;

    void deserialize(QDataStream &in) override;
};
