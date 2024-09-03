#include "dparallelogramitem.h"
#include "magpoint.h"

DParallelogramItem::DParallelogramItem(QGraphicsItem *parent)
	: DParallelogramItem(minRectSize, minRectSize, parent) {}

DParallelogramItem::DParallelogramItem(qreal w, qreal h, QGraphicsItem *parent)
	: DPolygonBase("", parent)
{
	modis.resize(1);
	for(int i = 0; i < 6; i++) mags->push_back(new MagPoint(this));
	polygon << QPointF(-w/4, -h/2) << QPointF(w, -h/2)
			<< QPointF(w/4, h/2) << QPointF(-w, h/2);
	updateAll();
}

void DParallelogramItem::updateMagPoint()
{
	QRectF rect = sizeRect();
	(*mags)[0]->setPos((polygon[0] + polygon[1]) / 2);
	(*mags)[1]->setPos((polygon[1] + polygon[2]) / 2);
	(*mags)[2]->setPos((polygon[2] + polygon[3]) / 2);
	(*mags)[3]->setPos((polygon[3] + polygon[0]) / 2);

	if(polygon[0].x() <= 0)
	{
		(*mags)[4]->setPos({0, rect.top()});
		(*mags)[5]->setPos({0, rect.bottom()});
	}
	else
	{
		QLineF line1(polygon[3], polygon[0]), line2({0, rect.top()}, {0, 0});
		QPointF interPoint;
		line1.intersects(line2, &interPoint);
		(*mags)[4]->setPos(interPoint);
		(*mags)[5]->setPos(-interPoint);
	}
}

void DParallelogramItem::updateModiPoint()
{
	modis[0] = polygon[0];
}

void DParallelogramItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(id);
	QRectF rc = sizeRect();
	p.setX(qMax(p.x(), rc.left()));
	p.setX(qMin(p.x(), rc.right()));
	polygon[0].setX(p.x());
	polygon[2].setX(-p.x());
	updateModiPoint();
	updateMagPoint();
	return;
}

//==============================================================================

void DParallelogramItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DPolygonBase::serialize(out, fa);
}

bool DParallelogramItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DPolygonBase::deserialize(in, fa)) return false;
	updateAll();
	return true;
}
