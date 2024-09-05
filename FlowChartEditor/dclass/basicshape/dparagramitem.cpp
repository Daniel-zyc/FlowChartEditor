#include "dparagramitem.h"
#include "../../magpoint.h"

DParagramItem::DParagramItem(QGraphicsItem *parent)
	: DParagramItem(minRectSize, minRectSize, parent) {}

DParagramItem::DParagramItem(qreal w, qreal h, QGraphicsItem *parent)
	: DPolygonBase("", parent)
{
	modis.resize(1);
	for(int i = 0; i < 6; i++) mags->push_back(new MagPoint(this));
	polygon << QPointF(-w/4, -h/2) << QPointF(w, -h/2)
			<< QPointF(w/4, h/2) << QPointF(-w, h/2);
	updateAll();
}

void DParagramItem::updateMagPoint()
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

void DParagramItem::updateModiPoint()
{
	modis[0] = polygon[0];
}

void DParagramItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(id);
	QRectF rc = sizeRect(); qreal nx = p.x();
	nx = qMax(nx, rc.left()); nx = qMin(nx, rc.right());
	polygon[0].setX(nx); polygon[2].setX(-nx);
	updateModiPoint();
	updateMagPoint();
}

//==============================================================================

void DParagramItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DPolygonBase::serialize(out, fa);
}

bool DParagramItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DPolygonBase::deserialize(in, fa)) return false;
	updateAll();
	return true;
}
