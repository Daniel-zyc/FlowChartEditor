#pragma once

#include "global.h"
#include "dabstractbase.h"

class DTextItem;

class DShapeBase : public DAbstractBase
{
public:
	enum { Type = DConst::DShapeBaseType };
	DShapeBase(QGraphicsItem *parent = nullptr);
	~DShapeBase() = default;

public:
	int type() const override { return Type; }

	QRectF boundingRect() const override;

	//==========================================================================
	virtual void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override = 0;
	//==========================================================================

	virtual void paintSelected(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	virtual void paintSelectRect(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
	virtual void paintRotPoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

public:
	virtual int checkInterPoint(QPointF p) const override;
	virtual int setInterPoint(QPointF p) override;
	virtual void interToPoint(QPointF p) override;

protected:
	virtual bool checkRotPoint(QPointF p) const;
	virtual bool setRotPoint(QPointF p);

	virtual void sizeToPoint(QPointF p, int id) override;
	virtual void rotToPoint(QPointF p);
	virtual void sizeRectUpdated();

protected:
	virtual QPainterPath shapeSelected() const override;
	virtual QPainterPath shapeShowMaged() const override;

	//==========================================================================
	virtual QRectF sizeRect() const = 0;
	virtual QPainterPath shapeNormal() const override = 0;
	virtual void sizeToRect(QRectF nrect) = 0;
	virtual void modiToPoint(QPointF p, int id) override = 0;
	//==========================================================================

	virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
	QRectF getResizeRect(const QPointF &p, int id);

public:
	DTextItem *textItem = nullptr;

protected:
	qreal rotPointRadius = 5, rotPointMargin = 30;
	QPointF rotPoint = {0, 0};
	
protected:
	QBrush rotPointBrush = QBrush(Qt::red, Qt::SolidPattern);
	QPen rotPointPen = QPen(Qt::black, 1, Qt::SolidLine);
	QBrush selectRectBrush = QBrush(Qt::transparent, Qt::SolidPattern);
	QPen selectRectPen = QPen(Qt::black, 1, Qt::DashLine);
	
private:
	int interactType = DConst::NONE;
};

