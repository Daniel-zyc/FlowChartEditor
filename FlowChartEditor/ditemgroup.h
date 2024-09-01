#pragma once

#include "global.h"

#include <QPainter>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>

class DItemGroup : public QGraphicsItemGroup
{
public:
	// enum { Type = DItemGroupType };
	DItemGroup(QGraphicsItem *parent = nullptr);

	// int type() const override { return Type; }

	QRectF boundingRect() const override;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

	void updateChilds() const;

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
	void updateBoundingRect();

	QRectF boundRect = QRectF(0, 0, 0, 0);
};

