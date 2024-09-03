#include "dlineitem.h"

DLineItem::DLineItem(QGraphicsItem *parent)
	: DLineItem({-minRectSize, -minRectSize}, {minRectSize, minRectSize}, parent) {}

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
	return QRectF(beginPoint, sz).normalized().adjusted(-r, -r, r, r);
}

void DLineItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

	painter->setBrush(Qt::NoBrush);
	painter->setPen(pen());
	painter->drawLine(QLineF(beginPoint, endPoint));

	qreal angle = getAngle(beginPoint, endPoint);
	drawArrow(painter, angle, endPoint, endArrowType);
}

void DLineItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(p); Q_UNUSED(id); return;
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

//================================

void DLineItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DLineBase::serialize(out, fa);
}

bool DLineItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	return DLineBase::deserialize(in, fa);
	updatePosition();
}
