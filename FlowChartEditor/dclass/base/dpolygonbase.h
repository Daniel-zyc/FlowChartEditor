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

	// 默认为绘制多边形
	virtual void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	// 默认为多边形的 boundingRect
	virtual QRectF sizeRect() const override;
	// 默认为多边形
	virtual QPainterPath shapeNormal() const override;
	// 默认为等比方大
	virtual void sizeToRect(QRectF nrect) override;

	virtual void updateAll();
	//==========================================================================
	virtual void updateMagPoint() = 0;
	virtual void updateModiPoint() = 0;
	//==========================================================================

protected:
	QPolygonF polygon;

public:
	// 多边形序列化，输出多边形
	virtual void serialize(QDataStream &out, const QGraphicsItem* fa) const override;
	virtual bool deserialize(QDataStream &in, QGraphicsItem* fa) override;
};
