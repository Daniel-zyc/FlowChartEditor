#pragma once

#include <QGraphicsLineItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

class DLineItem : public QGraphicsLineItem
{
public:
	DLineItem();

	QRectF boundingRect() const;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

