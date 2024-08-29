#include "dlineitem.h"

DLineItem::DLineItem(QGraphicsItem *parent)
	: DLineBase(parent) {}

DLineItem::DLineItem(QPointF begin, QPointF end, QGraphicsItem *parent)
	: DLineBase(parent)
{
	beginPoint = begin; endPoint = end;
	updatePosition();
}

QRectF DLineItem::boundingRect() const
{
	qreal r = maxPointRadius;
	QSizeF sz(endPoint.x() - beginPoint.x(), endPoint.y() - beginPoint.y());
	qDebug() << QRectF(beginPoint, sz).normalized().adjusted(-r, -r, r, r);
	return QRectF(beginPoint, sz).normalized().adjusted(-r, -r, r, r);
}

void DLineItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	setBrush(QBrush(Qt::black));
	painter->setBrush(brush());
	painter->setPen(pen());

	painter->drawLine(QLineF(beginPoint, endPoint));
}

void DLineItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(p); Q_UNUSED(id);
}

QPainterPath DLineItem::shapeNormal() const
{
	QGraphicsLineItem item(QLineF(beginPoint, endPoint));
	return item.shape();
}

void DLineItem::updateLine()
{
	return;
}
