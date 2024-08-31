#include "dlinebase.h"
#include "magpoint.h"
#include "serializer.h"

DLineBase::DLineBase(QGraphicsItem *parent)
	: DAbstractBase(parent)
{
	sizes.resize(2);
}

void DLineBase::paintSelected(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	paintSizePoint(painter, option, widget);
	paintModiPoint(painter, option, widget);
}

QPainterPath DLineBase::shapeSelected() const
{
	QPainterPath pth; pth.setFillRule(Qt::WindingFill);

	qreal r = sizePointRadius;
	for(const QPointF &mp : sizes) pth.addEllipse(mp, r, r);

	r = modiPointRadius;
	for(const QPointF &mp : modis) pth.addEllipse(mp, r, r);
	return pth;
}

QPainterPath DLineBase::shapeShowMaged() const
{
	QPainterPath pth; pth.setFillRule(Qt::WindingFill);

	qreal r = magPointCollideRadius;
	for(MagPoint *mp : *mags) pth.addEllipse(mp->pos, r, r);
	return pth;
}

int DLineBase::checkInterPoint(QPointF p) const
{
	p = mapFromScene(p);
	if(!isSelected()) return DConst::NONE;
	if(checkModiPoint(p)) return DConst::MODI;
	if(checkSizePoint(p)) return DConst::SIZE;
	return DConst::NONE;
}

int DLineBase::setInterPoint(QPointF p)
{
	p = mapFromScene(p);
	if(!isSelected()) return interactType = DConst::NONE;
	if(setModiPoint(p)) return interactType = DConst::MODI;
	if(setSizePoint(p)) return interactType = DConst::SIZE;
	return interactType = DConst::NONE;
}

void DLineBase::interToPoint(QPointF p, MagPoint *mp)
{
	if(interactType == DConst::NONE) return;

	prepareGeometryChange();
	switch(interactType)
	{
		case DConst::MODI:
			modiToPointPre(mapFromScene(p));
			break;
		case DConst::SIZE:
			sizeToPointPre(mapFromScene(p), mp);
			break;
	}
	update();
}

void DLineBase::setInsertItem()
{
	interactType = DConst::SIZE;
	sizePointId = DConst::ED - 1;
}

void DLineBase::linkBegin(MagPoint *mp)
{
	beginMag = mp;
	mp->addLine(this);
	updatePosition();
}

void DLineBase::linkEnd(MagPoint *mp)
{
	endMag = mp;
	mp->addLine(this);
	updatePosition();
}

void DLineBase::unlinkBegin()
{
	if(beginMag)
	{
		beginPoint = beginMag->mapToItem(this);
		beginMag->deleteLine(this);
		beginMag = nullptr;
	}
	updatePosition();
}

void DLineBase::unlinkEnd()
{
	if(endMag)
	{
		endPoint = endMag->mapToItem(this);
		endMag->deleteLine(this);
		endMag = nullptr;
	}
	updatePosition();
}

void DLineBase::unlinkMag(MagPoint* mp)
{
	if(beginMag == mp)
	{
		beginPoint = beginMag->mapToItem(this);
		beginMag = nullptr;
	}
	if(endMag == mp)
	{
		endPoint = endMag->mapToItem(this);
		endMag = nullptr;
	}
	updatePosition();
}

void DLineBase::sizeToPoint(QPointF p, int id, MagPoint *mp)
{
	switch(id)
	{
		case DConst::ST - 1 :
			if(mp) linkBegin(mp);
			else
			{
				unlinkBegin();
				beginPoint = p;
			}
			break;
		case DConst::ED - 1 :
			if(mp) linkEnd(mp);
			else
			{
				unlinkEnd();
				endPoint = p;
			}
			break;
	}
	updatePosition();
}

void DLineBase::updatePosition()
{
	// qDebug() << beginMag << " " << endMag;
	if(beginMag) beginPoint = beginMag->mapToItem(this);
	if(endMag) endPoint = endMag->mapToItem(this);
	sizes[DConst::ST - 1] = beginPoint;
	sizes[DConst::ED - 1] = endPoint;
	prepareGeometryChange();
	updateLine();
	update();
}

void DLineBase::setBeginArrowType(int type)
{
	beginArrowType = type;
	update();
}

void DLineBase::setEndArrowType(int type)
{
	endArrowType = type;
	update();
}

//===========================================

void DLineBase::serialize(QDataStream &out) const{
    // qDebug() << "line base serializing";
    DAbstractBase::serialize(out);

    out << reinterpret_cast<qintptr>(this);

	out << beginArrowType << endArrowType;
	out << beginPoint << endPoint;

    out << brush() << pen();

    qintptr beginPtr = beginMag != nullptr? reinterpret_cast<qintptr>(beginMag) : qintptr(-1);
    qintptr endPtr = endMag != nullptr? reinterpret_cast<qintptr>(endMag) : qintptr(-1);
    out << beginPtr << endPtr;
}

void DLineBase::deserialize(QDataStream &in){
    // qDebug() << "line base deserializing";
    DAbstractBase::deserialize(in);

	qintptr thisPtr; in >> thisPtr;
    Serializer::instance().PtrToLineBase.insert(thisPtr, this);

	in >> beginArrowType >> endArrowType;
	in >> beginPoint >> endPoint;
	updatePosition();

	QBrush qb; in >> qb; setBrush(qb);
	QPen qp; in >> qp; setPen(qp);

    qintptr beginptr,endptr;
    in >> beginptr >> endptr;

    if(beginptr != -1) Serializer::instance().LineBaseToBeginMagPonint.insert(this,beginptr);
    if(endptr != -1) Serializer::instance().LineBaseToEndMagPoint.insert(this,endptr);
}
