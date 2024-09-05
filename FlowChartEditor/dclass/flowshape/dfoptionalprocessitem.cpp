#include "dfoptionalprocessitem.h"
#include "../../magpoint.h"

DFOptionalProcessItem::DFOptionalProcessItem(QGraphicsItem *parent)
	: DFOptionalProcessItem(minRectSize, minRectSize, parent) {}

DFOptionalProcessItem::DFOptionalProcessItem(qreal w, qreal h, QGraphicsItem *parent)
	: DShapeBase("", parent)
{
	modis.resize(2);
	for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
	rect = QRectF(-w/2, -h/2, w, h);
	updateAll();
}

void DFOptionalProcessItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	painter->setBrush(brush());
	painter->setPen(pen());
	painter->drawRoundedRect(rect, rect.width() * ratiox,
							 rect.height() * ratioy);
}

QRectF DFOptionalProcessItem::sizeRect() const
{
	return rect;
}

QPainterPath DFOptionalProcessItem::shapeNormal() const
{
	QPainterPath pth;
	pth.addRoundedRect(rect, rect.width() * ratiox,
							 rect.height() * ratioy);
	return pth;
}

void DFOptionalProcessItem::updateMagPoint()
{
	(*mags)[0]->setPos({rect.left(), 0});
	(*mags)[1]->setPos({rect.right(), 0});

	(*mags)[2]->setPos({0, rect.top()});
	(*mags)[3]->setPos({0, rect.bottom()});
}

void DFOptionalProcessItem::updateModiPoint()
{
	modis[0] = {rect.left() + rect.width() * ratiox, rect.top()};
	modis[1] = {rect.left(), rect.top() + rect.height() * ratioy};
}

void DFOptionalProcessItem::sizeToRect(QRectF nrect)
{
	rect = nrect; updateAll();
}

void DFOptionalProcessItem::modiToPoint(QPointF p, int id)
{
	switch(id)
	{
		case 0:
			ratiox = (p.x() - rect.left()) / rect.width();
			ratiox = qMin(ratiox, 0.5);
			ratiox = qMax(0.0, ratiox);
			updateModiPoint();
			break;
		case 1:
			ratioy = (p.y() - rect.top()) / rect.height();
			ratioy = qMin(ratioy, 0.5);
			ratioy = qMax(0.0, ratioy);
			updateModiPoint();
			break;
	}
	return;
}

void DFOptionalProcessItem::updateAll()
{
	updateSizePoint();
	updateMagPoint();
	updateModiPoint();
}

//====================================

void DFOptionalProcessItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DShapeBase::serialize(out, fa);

	out << rect << ratiox << ratioy;
}

bool DFOptionalProcessItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DShapeBase::deserialize(in, fa)) return false;

	in >> rect >> ratiox >> ratioy;
	updateAll();
	return true;
}
