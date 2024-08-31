#include "dtriitem.h"
#include "magpoint.h"

DTriItem::DTriItem(QGraphicsItem *parent)
	: DTriItem(minRectSize, minRectSize, parent) {}

DTriItem::DTriItem(qreal w, qreal h, QGraphicsItem *parent)
	: DPolygonBase(parent)
{
	modis.resize(1);
	for(int i = 0; i < 3; i++) mags->push_back(new MagPoint(this));

	QPolygonF poly;
	poly << QPointF(0, -h/2) << QPointF(w/2, h/2) << QPointF(-w/2, h/2);
	setPoly(poly);
}

void DTriItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(id);
	QRectF rc = sizeRect();
	QPolygonF poly = polygon;
	p.setX(qMax(p.x(), rc.left()));
	p.setX(qMin(p.x(), rc.right()));
	poly[0].setX(p.x());
	setPoly(poly);
}

void DTriItem::updateMagPoint()
{
	(*mags)[0]->pos = (polygon[0] + polygon[1]) / 2;
	(*mags)[1]->pos = (polygon[1] + polygon[2]) / 2;
	(*mags)[2]->pos = (polygon[2] + polygon[0]) / 2;
}

void DTriItem::updateModiPoint()
{
	modis[0] = polygon[0];
}

//===================================
void DTriItem::serialize(QDataStream &out) const{
    qDebug() << "DTriItem serializing";
    DPolygonBase::serialize(out);
}

void DTriItem::deserialize(QDataStream &in){
    qDebug() << "DTriItem deserializing";
    DPolygonBase::deserialize(in);
}
