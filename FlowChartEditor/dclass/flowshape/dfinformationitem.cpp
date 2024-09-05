#include "dfinformationitem.h"
#include "../../magpoint.h"

DFInformationItem::DFInformationItem(QGraphicsItem *parent)
	: DFInformationItem(minRectSize, minRectSize, parent) {}

DFInformationItem::DFInformationItem(qreal w, qreal h, QGraphicsItem *parent)
	: DShapeBase("", parent)
{
	for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
	rect = QRectF(-w/2, -h/2, w, h);
	updateAll();
}

void DFInformationItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	painter->setBrush(brush());
	painter->setPen(pen());
	painter->drawPath(path);
}

QRectF DFInformationItem::sizeRect() const
{
	return rect;
}

QPainterPath DFInformationItem::shapeNormal() const
{
	return path;
}

void DFInformationItem::updateMagPoint()
{
    (*mags)[0]->pos = {rect.left(), 0};
    (*mags)[1]->pos = {rect.right(), 0};

	(*mags)[2]->pos = {0, rect.top() * 4 / 5};
	(*mags)[3]->pos = {0, rect.bottom() * 4 / 5};
}

void DFInformationItem::updatePath()
{
	path.clear();

	QPointF tl = rect.topLeft(), tr = rect.topRight();
	QPointF bl = rect.bottomLeft(), br = rect.bottomRight();
	qreal w = rect.width(), h = rect.height();

	path.moveTo(tl + QPointF(0, h / 10));

	qreal waveLength = w / 2;

	QPointF controlPoint1 = QPointF(tl.x() + waveLength / 2, tl.y() + h / 4);
	QPointF controlPoint2 = QPointF(tr.x() - waveLength / 2, tl.y() - h / 10);
	QPointF controlPoint3 = QPointF(br.x() - waveLength / 2, bl.y() - h / 4);
	QPointF controlPoint4 = QPointF(bl.x() + waveLength / 2, bl.y() + h / 10);

	QPointF endPoint1 = (tl + tr) / 2 + QPointF(0, h / 10);
	QPointF endPoint2 = tr + QPointF(0, h / 10);
	QPointF endPoint3 = (bl + br) / 2 - QPointF(0, h / 10);
	QPointF endPoint4 = bl - QPointF(0, h / 10);

	path.quadTo(controlPoint1, endPoint1);
	path.quadTo(controlPoint2, endPoint2);

	path.lineTo(br - QPointF(0, h / 10));

	path.quadTo(controlPoint3, endPoint3);
	path.quadTo(controlPoint4, endPoint4);

	path.closeSubpath();
}

void DFInformationItem::sizeToRect(QRectF nrect)
{
	rect = nrect; updateAll();
}

void DFInformationItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(p); Q_UNUSED(id); return;
}

void DFInformationItem::updateAll()
{
    updateSizePoint();
	updatePath();
    updateMagPoint();
}

//==============================================================================

void DFInformationItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DShapeBase::serialize(out, fa);
	out << rect;
}

bool DFInformationItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DShapeBase::deserialize(in, fa)) return false;
	in >> rect;
	updateAll();
	return true;
}
