#pragma once

#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>

class DItemGroup : public QGraphicsItemGroup
{
public:
	DItemGroup(QGraphicsItem *parent = nullptr);

	QRectF boundingRect() const override;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

