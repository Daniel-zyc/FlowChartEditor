#include "dlineitem.h"
#include "global.h"

#include <QLine>
#include <QPen>
#include <QBrush>

DLineItem::DLineItem()
	: QGraphicsLineItem()
{

}

QRectF DLineItem::boundingRect() const
{
	qreal extra = (pen().width() + 20) / 2.0;

	return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
									  line().p2().y() - line().p1().y()))
		.normalized()
		.adjusted(-extra, -extra, extra, extra);
}

void DLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
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
	}
}
