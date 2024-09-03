#include "dfdocumentitem.h"
#include "magpoint.h"

DFDocumentItem::DFDocumentItem(QGraphicsItem *parent)
	: DFDocumentItem(minRectSize, minRectSize, parent) {}

DFDocumentItem::DFDocumentItem(qreal w, qreal h, QGraphicsItem *parent)
	: DShapeBase("", parent)
{
	for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
	rect = QRectF(-w/2, -h/2, w, h);
	updateAll();
}

void DFDocumentItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	painter->setBrush(brush());
	painter->setPen(pen());
	painter->drawPath(path);
}

QRectF DFDocumentItem::sizeRect() const
{
	return rect;
}

QPainterPath DFDocumentItem::shapeNormal() const
{
	return path;
}

void DFDocumentItem::updateMagPoint()
{
	(*mags)[0]->setPos({rect.left(), -rect.height() / 10});
	(*mags)[1]->setPos({rect.right(), -rect.height() / 10});

	(*mags)[2]->setPos({0, rect.top()});
	(*mags)[3]->setPos({0, rect.bottom() * 4 / 5});
}

void DFDocumentItem::updatePath()
{
	path.clear();

	path.moveTo(rect.topLeft());
	path.lineTo(rect.topRight());
	path.lineTo(rect.bottomRight() - QPointF(0, rect.height() / 5));

	qreal waveLength = rect.width() / 2;

	QPointF startPoint = rect.bottomRight() - QPointF(0, rect.height() / 5);
	QPointF controlPoint, endPoint;

	controlPoint = QPointF(startPoint.x() - waveLength / 2, startPoint.y() - 2);
	endPoint = QPointF(startPoint.x() - waveLength, startPoint.y() + rect.height() / 10);
	path.quadTo(controlPoint, endPoint);
	startPoint = endPoint;

	controlPoint = QPointF(startPoint.x() - waveLength / 2, startPoint.y() + rect.height() / 6 * 1);
	endPoint = QPointF(startPoint.x() - waveLength, startPoint.y());
	path.quadTo(controlPoint, endPoint);
	startPoint = endPoint;

	path.lineTo(rect.bottomLeft() - QPointF(0, rect.height() / 10));
	path.closeSubpath();
}

void DFDocumentItem::sizeToRect(QRectF nrect)
{
	rect = nrect; updateAll();
}

void DFDocumentItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(p); Q_UNUSED(id); return;
}

void DFDocumentItem::updateAll()
{
	updateSizePoint();
	updatePath();
	updateMagPoint();
}

//==============================================================================

void DFDocumentItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DShapeBase::serialize(out, fa);

	out << rect;
}

bool DFDocumentItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DShapeBase::deserialize(in, fa)) return false;

	in >> rect;
	updateAll();
	return true;
}
