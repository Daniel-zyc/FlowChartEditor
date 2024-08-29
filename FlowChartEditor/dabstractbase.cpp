#include "magpoint.h"
#include "dabstractbase.h"

DAbstractBase::DAbstractBase(QGraphicsItem *parent)
	: QAbstractGraphicsShapeItem(parent)
{
	maxPointRadius = qMax(qMax(magPointRadius,sizePointRadius), qMax(magPointCollideRadius, modiPointRadius));
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable
			 | QGraphicsItem::ItemSendsGeometryChanges);
	mags = new QList<MagPoint*>();
}

QPainterPath DAbstractBase::shape() const
{
	QPainterPath pth; pth.setFillRule(Qt::WindingFill);
	pth.addPath(shapeNormal());
	if(isSelected()) pth.addPath(shapeSelected());
	if(showMagPoint) pth.addPath(shapeShowMaged());
	return pth;
}

void DAbstractBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	paintShape(painter, option, widget);
	if(isSelected()) paintSelected(painter, option, widget);
	if(showMagPoint) paintMagPoint(painter, option, widget);
}

void DAbstractBase::paintModiPoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	painter->setBrush(modiPointBrush);
	painter->setPen(modiPointPen);

	qreal r = modiPointRadius;
	for(const QPointF& pt : modis) painter->drawEllipse(pt, r, r);
}

void DAbstractBase::paintSizePoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	painter->setBrush(sizePointBrush);
	painter->setPen(sizePointPen);

	qreal r = sizePointRadius;
	for(const QPointF& pt : sizes) painter->drawEllipse(pt, r, r);
}

void DAbstractBase::paintMagPoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	painter->setBrush(magPointCollideBursh);
	painter->setPen(magPointColidePen);

	qreal r = magPointCollideRadius;
	for(MagPoint* mag : *mags) painter->drawEllipse(mag->pos, r, r);

	painter->setBrush(magPointBrush);
	painter->setPen(magPointPen);

	r = magPointRadius;
	for(MagPoint* mag : *mags) painter->drawEllipse(mag->pos, r, r);
}

void DAbstractBase::modiToPointPre(QPointF p)
{
	if(modiPointId == -1) return;
	
	prepareGeometryChange();
	modiToPoint(p, modiPointId);
	updateAllLinkLines();
	update();
}

void DAbstractBase::sizeToPointPre(QPointF p)
{
	if(sizePointId == -1) return;

	prepareGeometryChange();
	sizeToPoint(p, sizePointId);
	updateAllLinkLines();
	update();
}

int DAbstractBase::checkModiPoint(QPointF p) const
{
	qreal r = modiPointRadius;
	for(int i = 0; i < modis.size(); i++)
	{
		const QPointF& mp = modis[i];
		if(QRectF(mp.x() - r, mp.y() - r, 2*r, 2*r).contains(p))
			return i;
	}
	return -1;
}

bool DAbstractBase::setModiPoint(QPointF p)
{
	qreal r = modiPointRadius, minDist = 0;
	modiPointId = -1;
	for(int i = 0; i < modis.size(); i++)
	{
		const QPointF& mp = modis[i];
		if(QRectF(mp.x() - r, mp.y() - r, 2*r, 2*r).contains(p))
		{
			qreal dist = QLineF(p, mp).length();
			if(modiPointId == -1 || minDist > dist)
			{
				modiPointId = i;
				minDist = dist;
			}
		}
	}
	return modiPointId != -1;
}

int DAbstractBase::checkSizePoint(QPointF p) const
{
	qreal r = modiPointRadius;
	for(int i = 0; i < sizes.size(); i++)
	{
		const QPointF& mp = sizes[i];
		if(QRectF(mp.x() - r, mp.y() - r, 2*r, 2*r).contains(p))
			return i;
	}
	return -1;
}

bool DAbstractBase::setSizePoint(QPointF p)
{
	qreal r = sizePointRadius, minDist = 0;
	sizePointId = -1;
	for(int i = 0; i < sizes.size(); i++)
	{
		const QPointF& mp = sizes[i];
		if(QRectF(mp.x() - r, mp.y() - r, 2*r, 2*r).contains(p))
		{
			qreal dist = QLineF(p, mp).length();
			if(sizePointId == -1 || minDist > dist)
			{
				sizePointId = i;
				minDist = dist;
			}
		}
	}
	return sizePointId != -1;
}

void DAbstractBase::setShowMagPoint(bool show)
{
	prepareGeometryChange();
	showMagPoint = show;
	update();
}

bool DAbstractBase::checkMagPoint(QPointF p) const
{
	p = mapFromScene(p);
	qreal r = magPointCollideRadius;
	for(int i = 0; i < mags->size(); i++)
	{
		const QPointF& mp = (*mags)[i]->pos;
		if(QRectF(mp.x() - r, mp.y() - r, 2*r, 2*r).contains(p))
			return i;
	}
	return -1;
}

MagPoint* DAbstractBase::getMagPoint(QPointF p)
{
	p = mapFromScene(p);
	qreal r = magPointCollideRadius, minDist = 0;
	int tmpId = -1;
	for(int i = 0; i < mags->size(); i++)
	{
		const QPointF& mp = (*mags)[i]->pos;
		if(QRectF(mp.x() - r, mp.y() - r, 2*r, 2*r).contains(p))
		{
			qreal dist = QLineF(p, mp).length();
			if(tmpId == -1 || minDist > dist)
			{
				tmpId = i;
				minDist = dist;
			}
		}
	}
	if(tmpId != -1) return (*mags)[tmpId];
	else return nullptr;
}

void DAbstractBase::updateAllLinkLines()
{
	for(MagPoint* mag : *mags) mag->updateLines();
}
