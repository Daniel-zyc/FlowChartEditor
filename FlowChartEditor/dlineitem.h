// #pragma once

// #include "global.h"
// #include "dlinebase.h"

// #include <QGraphicsLineItem>
// #include <QPainter>
// #include <QStyleOptionGraphicsItem>
// #include <QRectF>
// #include <QGraphicsSceneMouseEvent>

// class DShapeBase;

// class DLineItem : public DLineBase
// {
// public:
// 	enum { Type = DConst::DShapeBaseType };
// 	DLineItem();

// 	int type() const override { return Type; }

// 	QRectF boundingRect() const override;

// 	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
// 	QPainterPath shape() const override
// 	{
// 		qreal arrowSize = 20;
// 		double angle = std::atan2(-line().dy(), line().dx());
// 		QPointF arrowP1 = line().p1() + QPointF(sin(angle + PI / 3) * arrowSize, cos(angle + PI / 3) * arrowSize);
// 		QPointF arrowP2 = line().p1() + QPointF(sin(angle + PI - PI / 3) * arrowSize, cos(angle + PI - PI / 3) * arrowSize);

// 		QPolygonF arrowHead;
// 		arrowHead << line().p1() << arrowP1 << arrowP2;
// 		QPainterPath path = QGraphicsLineItem::shape();
// 		path.addPolygon(arrowHead);
// 		return path;
// 	}

// 	void updatePosition() override;

// 	int pressPoint = 0;

// protected:
// 	// void mousePressEvent(QGraphicsSceneMouseEvent *event) override
// 	// {
// 	// 	QPointF p = event->pos();
// 	// 	if(QRectF(p.x() - 5, p.y() - 5, 10, 10).contains(line().p1()))
// 	// 	{
// 	// 		event->accept();
// 	// 		pressPoint = 1;
// 	// 		return;
// 	// 	}

// 	// 	if(QRectF(p.x() - 5, p.y() - 5, 10, 10).contains(line().p2()))
// 	// 	{
// 	// 		event->accept();
// 	// 		pressPoint = 2;
// 	// 		return;
// 	// 	}

// 	// 	QGraphicsLineItem::mousePressEvent(event);
// 	// }

// 	// void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override
// 	// {
// 	// 	QPointF p = event->pos();
// 	// 	if(pressPoint == 1)
// 	// 	{
// 	// 		startShape = nullptr;
// 	// 		event->accept();
// 	// 		QLineF tmp = line();
// 	// 		tmp.setP1(p);
// 	// 		setLine(tmp);
// 	// 		return;
// 	// 	}
// 	// 	if(pressPoint == 2)
// 	// 	{
// 	// 		endShape = nullptr;
// 	// 		event->accept();
// 	// 		QLineF tmp = line();
// 	// 		tmp.setP2(p);
// 	// 		setLine(tmp);
// 	// 		return;
// 	// 	}

// 	// 	QGraphicsLineItem::mouseMoveEvent(event);
// 	// }

// 	// void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override
// 	// {
// 	// 	pressPoint = 0;
// 	// 	QGraphicsLineItem::mouseReleaseEvent(event);
// 	// }
// };

