#pragma once

#include "global.h"
#include "magpoint.h"
#include "dtextitem.h"

#include <QVector>
#include <QAbstractGraphicsShapeItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>
#include <QPainterPath>
#include <QStyle>
#include <QPainter>
#include <QBrush>
#include <QPen>

class DLineItem;
class MagPoint;

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
	enum { Type = UserTypes::DShapeBaseType };
	DShapeBase(QGraphicsItem *parent = nullptr);
	~DShapeBase() = default;

public:
	int type() const override { return Type; }

	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

	//--------------------------------------------------------------------------
	virtual void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) = 0;
	//--------------------------------------------------------------------------

	virtual void paintSelectRect(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
	virtual void paintSizePoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
	virtual void paintMagPoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
	virtual void paintModifyPoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

public:
	bool checkModiPoint(QPointF p, bool isItemCord = false) const;
	bool setModiPoint(QPointF p, bool isItemCord = false);

	bool checkSizePoint(QPointF p, bool isItemCord = false) const;
	bool setSizePoint(QPointF p, bool isItemCord = false);

	bool checkMagPoint(QPointF p, bool isItemCord = false) const;
	void setShowMagPoint(bool show = true);
	MagPoint* getMagPoint(QPointF p, bool isItemCord = false);

	void resizeToPoint(QPointF p, bool isItemCord = false);
	void modiToPoint(QPointF p, bool isItemCord = false);

protected:
	QPainterPath shapeSelected() const;

	//--------------------------------------------------------------------------
	virtual QRectF sizeRect() const = 0;
	virtual QPainterPath shapeNormal() const = 0;
	virtual void resizeToRect(QRectF nrect) = 0;
	virtual void modifyToPoint(QPointF p, int id) = 0;
	//--------------------------------------------------------------------------

	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
	QRectF getResizeRect(const QPointF &p);
	void updateAllLinkLines();

protected:
	qreal maxPointRadius;
	qreal sizePointRadius = 5, magPointRadius = 5, modiPointRadius = 5;
	QVector<MagPoint> mags;
	QVector<QPointF> modis;
	
private:
	static QBrush selectedRectBrush;
	static QPen selectedRectPen;
	static QBrush sizePointBrush;
	static QPen sizePointPen;
	static QBrush magPointBrush;
	static QPen magPointPen;
	static QBrush modiPointBrush;
	static QPen modiPointPen;

	bool showMagPoint = false;
	ResizeOrient resizeOrient = ResizeOrient::NONE;
	int modiPointId = -1;
	InteractPoint activeInteractType = InteractPoint::NONE;

	DTextItem textItem;
};

