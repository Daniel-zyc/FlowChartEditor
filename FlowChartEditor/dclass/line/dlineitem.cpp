#include "dlineitem.h"

DLineItem::DLineItem(QGraphicsItem *parent)
	: DLineItem({-minRectSize, -minRectSize}, {minRectSize, minRectSize}, parent) {}

DLineItem::DLineItem(QPointF begin, QPointF end, QGraphicsItem *parent)
	: DLineBase(parent)
{
	beginPoint = begin; endPoint = end;
	updateAll();
}

QRectF DLineItem::boundingRect() const
{
	qreal r = maxLineRaidus;
	QSizeF sz(endPoint.x() - beginPoint.x(), endPoint.y() - beginPoint.y());
	return QRectF(beginPoint, sz).normalized().adjusted(-r, -r, r, r);
}

void DLineItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

	painter->setBrush(Qt::NoBrush);
	painter->setPen(pen());
	painter->drawLine(QLineF(beginPoint, endPoint));

    // painter->setBrush(Qt::red);
    // painter->drawPath(shape());

	qreal angle = getAngle(beginPoint, endPoint);
    drawEndArrow(painter, angle, endPoint, endArrowType);
    drawBeginArrow(painter, angle, beginPoint, beginArrowType);
}

void DLineItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(p); Q_UNUSED(id); return;
}

QPainterPath DLineItem::shapeNormal() const
{
	return path;
}

void DLineItem::updateLine()
{
	updateAll();
}

void DLineItem::updatePath()
{
	path.clear();
    /*
	QLineF vec(QPointF{0, 0}, endPoint - beginPoint);
	vec.setLength(qMax(pen().widthF(), sizePointRadius));
	vec = vec.normalVector();
	QPointF dir = vec.p2();
	QPolygonF poly;
	poly << (beginPoint + dir) << (endPoint + dir)
         << (endPoint - dir) << (beginPoint - dir);
	path.addPolygon(poly);
    */
    path.addPath(getFillPath(beginPoint,endPoint));
}

void DLineItem::updateAll()
{
	updateSizePoint();
	updatePath();
}

//================================

void DLineItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DLineBase::serialize(out, fa);
	// qDebug() << "debug: " << beginPoint - endPoint;
}

bool DLineItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DLineBase::deserialize(in, fa)) return false;
	// qDebug() << "debug: " << beginPoint - endPoint;
	updateAll();
    // updatePosition();
	// qDebug() << "endMag" << endMag;
	// qDebug() << "beginMag" << beginMag;
	return true;
}
