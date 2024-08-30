#include "magpoint.h"
#include "dabstractbase.h"

#include "serializer.h"

DAbstractBase::DAbstractBase(QGraphicsItem *parent)
	: QAbstractGraphicsShapeItem(parent)
{
    setBrush(QBrush(Qt::transparent, Qt::SolidPattern));
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable
			 | QGraphicsItem::ItemSendsGeometryChanges);
	mags = new QList<MagPoint*>();
}

DAbstractBase::~DAbstractBase()
{
	delete mags;
}

QPainterPath DAbstractBase::shape() const
{
	QPainterPath pth; pth.setFillRule(Qt::WindingFill);
	pth.addPath(shapeNormal());
	if(isSelected()) pth.addPath(shapeSelected());
	if(showMagPoint) pth.addPath(shapeShowMaged());
	return pth;
}

QPainterPath DAbstractBase::shapeSelected() const
{
	QPainterPath pth; pth.setFillRule(Qt::WindingFill);
	qreal r = modiPointRadius;
	for(const QPointF& pt : modis) pth.addEllipse(pt, r, r);
	r = sizePointRadius;
	for(const QPointF& pt : sizes) pth.addEllipse(pt, r, r);
	return pth;
}

QPainterPath DAbstractBase::shapeShowMaged() const
{
	QPainterPath pth;
	qreal r = magPointCollideRadius;
	for(MagPoint* mag : *mags) pth.addEllipse(mag->pos, r, r);
	return pth;
}

void DAbstractBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	paintShape(painter, option, widget);
	if(isSelected()) paintSelected(painter, option, widget);
	if(showMagPoint) paintMagPoint(painter, option, widget);
}

void DAbstractBase::paintSelected(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	paintSizePoint(painter, option, widget);
	paintModiPoint(painter, option, widget);
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

void DAbstractBase::sizeToPointPre(QPointF p, MagPoint *mp)
{
	if(sizePointId == -1) return;

	prepareGeometryChange();
	sizeToPoint(p, sizePointId, mp);
	updateAllLinkLines();
	update();
}

int DAbstractBase::checkModiPoint(QPointF p) const
{
	qreal r = modiPointRadius;
	for(int i = 0; i < modis.size(); i++)
	{
		if(DTool::inCircle(modis[i], r, p))
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
		qreal dist = DTool::dist(p, modis[i]);
		if(dist <= r && (modiPointId == -1 || minDist > dist))
		{
			modiPointId = i;
			minDist = dist;
		}
	}
	return modiPointId != -1;
}

int DAbstractBase::checkSizePoint(QPointF p) const
{
	qreal r = modiPointRadius;
	for(int i = 0; i < sizes.size(); i++)
	{
		if(DTool::inCircle(sizes[i], r, p))
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
		qreal dist = DTool::dist(p, sizes[i]);
		if(dist < r && (sizePointId == -1 || minDist > dist))
		{
			sizePointId = i;
			minDist = dist;
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
		if(DTool::inCircle((*mags)[i]->pos, r, p))
			return true;
	}
	return false;
}

MagPoint* DAbstractBase::getMagPoint(QPointF p)
{
	p = mapFromScene(p);
	qreal r = magPointCollideRadius, minDist = 0;
	MagPoint *ptr = nullptr;
	for(int i = 0; i < mags->size(); i++)
	{
		qreal dist = DTool::dist(p, (*mags)[i]->pos);
		if(dist <= r && (ptr == nullptr || minDist > dist))
		{
			ptr = (*mags)[i];
			minDist = dist;
		}
	}
	return ptr;
}

void DAbstractBase::updateAllLinkLines()
{
	for(MagPoint* mag : *mags) mag->updateLines();
}

//=======================================

void DAbstractBase::serialize(QDataStream &out) const{
    out << reinterpret_cast<qintptr>(this);

    if(mags == nullptr) out << static_cast<quint32>(0);
    else{

        out << static_cast<quint32>(mags->size());
        for(MagPoint *magPoint : *mags){
            out << reinterpret_cast<qintptr>(magPoint);
        }
    }
}

void DAbstractBase::deserialize(QDataStream &in){
    qintptr thisPtr;
    in >> thisPtr;
    Serializer::instance().PtrToQGraphicsItem.insert(thisPtr,this);

    quint32 magPointCount;
    in >> magPointCount;
    for(quint32 i = 0; i < magPointCount; i++){
        qintptr magPointPtr;
        in >> magPointPtr;
        Serializer::instance().DAbstractBaseToMagsPtr.insert(this, magPointPtr);
    }
}
