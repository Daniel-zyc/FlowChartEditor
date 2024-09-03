#include "dcurvelineitem.h"
#include "magpoint.h"

DCurveLineItem::DCurveLineItem(QGraphicsItem *parent)
	: DCurveLineItem({-minRectSize, -minRectSize}, {minRectSize, minRectSize}, parent) {
}

DCurveLineItem::DCurveLineItem(QPointF begin, QPointF end, QGraphicsItem *parent)
    : DLineBase(parent)
{
	modis.resize(1);
	beginPoint = begin; endPoint = end;
	tip = (beginPoint + endPoint) / 2;
	updatePath();
	updateModiPoint();
	updatePosition();
}

QRectF DCurveLineItem::boundingRect() const
{
	qreal r = maxPointRadius;
	QPolygonF poly; poly << beginPoint << endPoint << tip;
	return poly.boundingRect().adjusted(-r, -r, r, r);
}

void DCurveLineItem::updatePath()
{
	path.clear();

	path.moveTo(beginPoint);

	QPointF controlPoint;
	qreal waveHeight;

	int bulge = 1;
	if(endPoint.y() > beginPoint.y()) bulge = -bulge;
	if(qAbs(angle) > DConst::PI / 4 && qAbs(angle) < DConst::PI * 3 / 4) {
		bulge = -bulge;
		if(endPoint.x() < beginPoint.x() || endPoint.y() > beginPoint.y()) bulge = -bulge;
	}
	if(qAbs(angle) <= DConst::PI / 4 || qAbs(angle) >= DConst::PI * 3 / 4) {
		waveHeight = qAbs(beginPoint.y() - endPoint.y()) / 2;

		controlPoint = QPointF(beginPoint.x() * 1 / 10 + tip.x() * 9 / 10, beginPoint.y() * 1 / 10 + tip.y() * 9 / 10 + waveHeight * bulge);
		path.quadTo(controlPoint, tip);

		controlPoint = QPointF(tip.x() * 9 / 10 + endPoint.x() * 1 / 10, tip.y() * 9 / 10 + endPoint.y() * 1 / 10 - waveHeight * bulge);
		path.quadTo(controlPoint, endPoint);
	} else {
		waveHeight = qAbs(beginPoint.x() - endPoint.x()) / 2;

		controlPoint = QPointF(beginPoint.x() * 1 / 10 + tip.x() * 9 / 10 + waveHeight * bulge, beginPoint.y() * 1 / 10 + tip.y() * 9 / 10);
		path.quadTo(controlPoint, tip);

		controlPoint = QPointF(tip.x() * 9 / 10 + endPoint.x() * 1 / 10 - waveHeight * bulge, tip.y() * 9 / 10 + endPoint.y() * 1 / 10);
		path.quadTo(controlPoint, endPoint);
	}

	qDebug() << path;
}

void DCurveLineItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

	painter->setBrush(Qt::NoBrush);
	painter->setPen(pen());
	painter->drawPath(path);

	qreal arrowAngle;

	if(qAbs(angle) < DConst::PI / 4) arrowAngle = 0;
	else if(qAbs(angle) < DConst::PI * 3 / 4)
		arrowAngle = DConst::PI / 2 * (angle > 0 ? 1 : -1);
	else arrowAngle = DConst::PI;

	if((endPoint.x() > beginPoint.x() && tip.x() > endPoint.x())
	   || (endPoint.x() < beginPoint.x() && tip.x() < endPoint.x()))
		arrowAngle = (arrowAngle == 0 ? DConst::PI : 0);
	if((endPoint.y() > beginPoint.y() && tip.y() > endPoint.y())
	   || (endPoint.y() < beginPoint.y() && tip.y() < endPoint.y()))
		arrowAngle *= -1;

	drawArrow(painter, arrowAngle, endPoint, endArrowType);
}

void DCurveLineItem::updateModiPoint()
{
	modis[0] = tip;
}

void DCurveLineItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(id);
	if(qAbs(angle) < DConst::PI / 4 || qAbs(angle) > DConst::PI * 3 / 4) tip.setX(p.x());
	else tip.setY(p.y());
	updateModiPoint();
	updatePath();
}

QPainterPath DCurveLineItem::shapeNormal() const
{
	return path;
}

void DCurveLineItem::updateLine()
{
	tip = (beginPoint + endPoint) / 2;
	angle = getAngle(beginPoint, endPoint);
	updateModiPoint();
	updatePath();
}

//================================

// void DCurveItem::serialize(QDataStream &out) const{
//     // qDebug() << "DLineItem serializing";
//     DLineBase::serialize(out);

//     out << tip << angle;
// }

// void DCurveItem::deserialize(QDataStream &in){
//     // qDebug() << "DLineItem deserializing";
//     DLineBase::deserialize(in);

//     in << tip << angle;
// }
