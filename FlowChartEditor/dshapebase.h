#pragma once

#include "dlineitem.h"

#include <QAbstractGraphicsShapeItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>
#include <QPainterPath>
#include <QStyle>
#include <QPainter>
#include <QBrush>

class DLineItem;

enum class ResizeOrient
{
	NONE = 0,
	T, B, L, R,
	TL, TR, BL, BR
};

enum class InteractPoint
{
	NONE = 0,
	SIZE = 1, MAG = 2, MODIFY = 4
};

class DShapeBase : public QAbstractGraphicsShapeItem
{
public:
	enum { Type = UserType + 1 };
	DShapeBase(QGraphicsItem *parent = nullptr);

public:
	int type() const override { return Type; }

	virtual QRectF boundingRect() const override;

	virtual QPainterPath shapeNormal() const;
	virtual QPainterPath shapeSelected() const;
	virtual QPainterPath shape() const override;

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	virtual void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
	virtual void paintSelectRect(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
	virtual void paintSizePoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
	virtual void paintMagPoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
	virtual void paintModifyPoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

	virtual QRectF sizeRect() const = 0;

	virtual bool checkInteractPoint(const QPointF &p);
	virtual ResizeOrient checkSizePoint(const QPointF &p);

	virtual void setActiveInteractPoint(const QPointF &p);
	virtual void setActiveSizePoint(ResizeOrient orient);

	virtual QRectF getResizeRect(const QPointF &p);
	virtual void resizeToRect(QRectF nrect) = 0;

	virtual void resizeToPoint(const QPointF &p);
	virtual void updateLinkedArrow();

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
	qreal sizePointRadius;
	InteractPoint activeInteractType;
	ResizeOrient resizeOrient;

public:
	QList<DLineItem *> arrows;
};

