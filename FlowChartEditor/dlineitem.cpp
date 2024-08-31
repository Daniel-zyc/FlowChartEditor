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
	return QRectF(beginPoint, sz).normalized().adjusted(-r, -r, r, r);
}

void DLineItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

	QBrush qbrush = painter->brush();
	qbrush.setColor(pen().color());
	setBrush(qbrush);

	painter->setBrush(brush());
	painter->setPen(pen());

    painter->drawLine(QLineF(beginPoint, endPoint));
	// 此处应加入根据不同箭头类型进行绘制的代码
    drawArrow(painter, beginPoint, endPoint, endArrowType);
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

//================================

void DLineItem::serialize(QDataStream &out) const{
    // qDebug() << "DLineItem serializing";
    DLineBase::serialize(out);
}

void DLineItem::deserialize(QDataStream &in){
    // qDebug() << "DLineItem deserializing";
    DLineBase::deserialize(in);
}
