#pragma once

#include "global.h"
#include "magpoint.h"
#include "dshapebase.h"

#include <QGraphicsLineItem>
#include <QLineF>

class DShapeBase;
class MagPoint;

class DLineBase : public QGraphicsLineItem
{
public:
	enum { Type = UserTypes::DLineBaseType };
	DLineBase(QGraphicsItem *parent = nullptr);
	~DLineBase() = default;

public:
	int type() const override { return Type; }

	// virtual void resetLine(QLineF nline) = 0;
	virtual void updatePosition() = 0;

	MagPoint *startMag = nullptr, *endMag = nullptr;
};

