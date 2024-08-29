#pragma once

#include "global.h"
#include "dabstractbase.h"

#include <QGraphicsItem>
#include <QLineF>

class MagPoint;

class DLineBase : public DAbstractBase
{
public:
	enum { Type = DConst::DLineBaseType };
	DLineBase(QGraphicsItem *parent = nullptr);
	~DLineBase() = default;

public:
	virtual int type() const override { return Type; }

	virtual void updatePosition() = 0;

protected:
	static QBrush selectedRectBrush;
	static QPen selectedRectPen;
	static QBrush sizePointBrush;
	static QPen sizePointPen;
	
	QVector<QPointF> modis;

	QLineF line;
	MagPoint *startMag, *endMag;
};

