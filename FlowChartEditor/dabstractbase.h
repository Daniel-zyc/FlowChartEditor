#pragma once

#include "global.h"

#include <QPen>
#include <QBrush>
#include <QVector>
#include <QPointF>
#include <QPainter>
#include <QGraphicsItem>

class MagPoint;

class DAbstractBase : public QAbstractGraphicsShapeItem
{
public:
	enum { Type = DConst::DAbstractBase };
	DAbstractBase(QGraphicsItem *parent = nullptr);
	~DAbstractBase() = default;
	
public:
	int type() const override { return Type; }

	virtual QPainterPath shape() const override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

public:
	virtual void setShowMagPoint(bool show = true);
	virtual bool checkMagPoint(QPointF p) const;
	virtual MagPoint* getMagPoint(QPointF p);

	//==========================================================================
	virtual int checkInterPoint(QPointF p) const = 0;
	virtual int setInterPoint(QPointF p) = 0;
	virtual void interToPoint(QPointF p) = 0;
	//==========================================================================

protected:
	virtual void paintModifyPoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
	virtual void paintSizePoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
	virtual void paintMagPoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

	//==========================================================================
	virtual void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) = 0;
	virtual void paintSelected(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) = 0;
	//==========================================================================

protected:
	virtual void modiToPointPre(QPointF P);
	virtual void sizeToPointPre(QPointF p);
	
	//==========================================================================
	virtual void modiToPoint(QPointF p, int id) = 0;
	virtual void sizeToPoint(QPointF p, int id) = 0;
	//==========================================================================

	virtual int checkModiPoint(QPointF p) const;
	virtual bool setModiPoint(QPointF p);
	
	virtual int checkSizePoint(QPointF p) const;
	virtual bool setSizePoint(QPointF p);

	virtual void updateAllLinkLines();

	//==========================================================================
	virtual QPainterPath shapeSelected() const = 0;
	virtual QPainterPath shapeNormal() const = 0;
	virtual QPainterPath shapeShowMaged() const = 0;
	//==========================================================================
	

protected:
	qreal maxPointRadius = 0;
	qreal sizePointRadius = 5, magPointRadius = 5, magPointCollideRadius = 10, modiPointRadius = 5;
	QList<MagPoint*> *mags = nullptr;
	QList<QPointF> modis = QList<QPointF>(), sizes = QList<QPointF>();

protected:
	QBrush modiPointBrush = QBrush(Qt::yellow, Qt::SolidPattern);
	QPen modiPointPen = QPen(Qt::black, 1, Qt::SolidLine);
	QBrush sizePointBrush = QBrush(Qt::white, Qt::SolidPattern);
	QPen sizePointPen = QPen(Qt::black, 1, Qt::SolidLine);
	QBrush magPointBrush = QBrush(Qt::darkGray, Qt::SolidPattern);
	QPen magPointPen = QPen(Qt::darkGray, 1, Qt::SolidLine);
	QBrush magPointCollideBursh = QBrush(Qt::gray, Qt::SolidPattern);
	QPen magPointColidePen = QPen(Qt::gray, Qt::SolidLine);

private:
	bool showMagPoint = false;
	int modiPointId = -1, sizePointId = -1;
};

