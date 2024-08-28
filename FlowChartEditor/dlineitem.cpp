#include "dlineitem.h"
#include "global.h"

#include <QLine>
#include <QPen>
#include <QBrush>

DLineItem::DLineItem()
	: DLineBase()
{

}

QRectF DLineItem::boundingRect() const
{
	qreal extra = 20;

	return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
									  line().p2().y() - line().p1().y()))
		.normalized()
		.adjusted(-extra, -extra, extra, extra);
}

void DLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	qreal arrowSize = 20;

	double angle = std::atan2(-line().dy(), line().dx());

	QPointF arrowP1 = line().p1() + QPointF(sin(angle + PI / 3) * arrowSize, cos(angle + PI / 3) * arrowSize);
	QPointF arrowP2 = line().p1() + QPointF(sin(angle + PI - PI / 3) * arrowSize, cos(angle + PI - PI / 3) * arrowSize);

	QPolygonF arrowHead;
	arrowHead << line().p1() << arrowP1 << arrowP2;

	painter->setBrush(QBrush(Qt::black));
	painter->setPen(pen());

	painter->drawLine(line());
	painter->drawPolygon(arrowHead);

	if (isSelected()) {
		painter->setPen(QPen(Qt::black, 1, Qt::DashLine));
		QLineF myLine = line();
		myLine.translate(0, 4.0);
		painter->drawLine(myLine);
		myLine.translate(0,-8.0);
		painter->drawLine(myLine);
		painter->setPen(QPen(Qt::black, 2, Qt::SolidLine));
		painter->setBrush(QBrush(Qt::white));
		painter->drawEllipse(line().p1(), 5, 5);
		painter->drawEllipse(line().p2(), 5, 5);
	}
}

void DLineItem::updatePosition()
{
	QPointF pa = line().p1(), pb = line().p2();
	if(startMag) pa = startMag->mapToItem(this);
	if(endMag) pb = endMag->mapToItem(this);
	setLine(QLineF(pa, pb));
}
