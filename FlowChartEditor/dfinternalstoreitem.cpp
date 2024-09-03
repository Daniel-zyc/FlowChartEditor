#include "dfinternalstoreitem.h"
#include "magpoint.h"

DFInternalStoreItem::DFInternalStoreItem(QGraphicsItem *parent)
	: DFInternalStoreItem(minRectSize, minRectSize, parent) {}

DFInternalStoreItem::DFInternalStoreItem(qreal w, qreal h, QGraphicsItem *parent)
	: DShapeBase("", parent)
{
	modis.resize(2);
	for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
	rect = QRectF(-w/2, -h/2, w, h);
	updateAll();
}

void DFInternalStoreItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	painter->setBrush(brush());
	painter->setPen(pen());

	qreal hory = rect.top() + rect.height() * ratioy;
	qreal verx = rect.left() + rect.width() * ratiox;
	painter->drawRect(rect);
	painter->drawLine(QPointF(rect.left(), hory), QPointF(rect.right(), hory));
	painter->drawLine(QPointF(verx, rect.top()), QPointF(verx, rect.bottom()));
}

QRectF DFInternalStoreItem::sizeRect() const
{
	return rect;
}

QPainterPath DFInternalStoreItem::shapeNormal() const
{
	QPainterPath pth; pth.addRect(rect); return pth;
}

void DFInternalStoreItem::updateMagPoint()
{
	(*mags)[0]->setPos({rect.left(), 0});
	(*mags)[1]->setPos({rect.right(), 0});

	(*mags)[2]->setPos({0, rect.top()});
	(*mags)[3]->setPos({0, rect.bottom()});
}

void DFInternalStoreItem::updateModiPoint()
{
	modis[0] = {rect.left() + rect.width() * ratiox, rect.top()};
	modis[1] = {rect.left(), rect.top() + rect.height() * ratioy};
}

void DFInternalStoreItem::sizeToRect(QRectF nrect)
{
	rect = nrect; updateAll();
}

void DFInternalStoreItem::modiToPoint(QPointF p, int id)
{
	switch(id)
	{
		case 0:
			ratiox = (p.x() - rect.left()) / rect.width();
			ratiox = qMin(ratiox, 1.0);
			ratiox = qMax(0.0, ratiox);
			updateModiPoint();
			break;
		case 1:
			ratioy = (p.y() - rect.top()) / rect.height();
			ratioy = qMin(ratioy, 1.0);
			ratioy = qMax(0.0, ratioy);
			updateModiPoint();
			break;
	}
}

void DFInternalStoreItem::updateAll()
{
	updateSizePoint();
	updateMagPoint();
	updateModiPoint();
}

//==============================================================================

void DFInternalStoreItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DShapeBase::serialize(out, fa);

	out << rect << ratiox << ratioy;
}

bool DFInternalStoreItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DShapeBase::deserialize(in, fa)) return false;

	in >> rect >> ratiox >> ratioy;
	updateAll();
	return true;
}

