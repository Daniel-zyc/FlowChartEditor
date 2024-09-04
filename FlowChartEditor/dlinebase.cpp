#include "dlinebase.h"
#include "magpoint.h"
#include "serializer.h"

DLineBase::DLineBase(QGraphicsItem *parent)
	: DAbstractBase(parent)
{
	sizes.resize(2);
	isScaleable = false;
	isRotateable = false;
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

void DLineBase::linkBeginUpdate(MagPoint *mp)
{
	if(!mp) return;
	beginMag = mp;
	mp->addLine(this);
	updatePosition();
}

void DLineBase::linkEndUpdate(MagPoint *mp)
{
	if(!mp) return;
	endMag = mp;
	mp->addLine(this);
	updatePosition();
}

void DLineBase::linkBegin(MagPoint *mp)
{
	if(!mp) return;
	beginMag = mp; mp->addLine(this); beginPoint = mp->mapToItem(this);
}

void DLineBase::linkEnd(MagPoint *mp)
{
	if(!mp) return;
	endMag = mp; mp->addLine(this); endPoint = mp->mapToItem(this);
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
			if(mp && mp != endMag) linkBeginUpdate(mp);
			else
			{
				// qDebug() << "unlinkBegin";
				unlinkBegin();
				beginPoint = p;
			}
			break;
		case DConst::ED - 1 :
			if(mp && mp != beginMag) linkEndUpdate(mp);
			else
			{
				// qDebug() << "unlinkEnd";
				unlinkEnd();
				endPoint = p;
			}
			break;
	}
	updatePosition();
}

void DLineBase::updatePosition()
{
	if(beginMag) beginPoint = beginMag->mapToItem(this);
	if(endMag) endPoint = endMag->mapToItem(this);
	updateSizePoint();
	prepareGeometryChange();
	updateLine();
	update();
}

void DLineBase::updateSizePoint()
{
	sizes[DConst::ST - 1] = beginPoint;
	sizes[DConst::ED - 1] = endPoint;
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

qreal DLineBase::getAngle(const QPointF &beginPoint, const QPointF &endPoint)
{
    QLineF line(beginPoint, endPoint);
    double angle = atan2(line.dy(), line.dx());
    return angle;
}

void DLineBase::drawArrow(QPainter *painter, double angle, const QPointF &endPoint, int arrowType)
{
	qreal arrowSize = pen().widthF() * 10;
	QPointF arrowP1 = endPoint - QPointF(cos(angle + DConst::PI / 6) * arrowSize,
										 sin(angle + DConst::PI / 6) * arrowSize);
	QPointF arrowP2 = endPoint - QPointF(cos(angle - DConst::PI / 6) * arrowSize,
										 sin(angle - DConst::PI / 6) * arrowSize);

	QBrush qbrush = brush(); qbrush.setColor(pen().color()); setBrush(qbrush);
	painter->setPen(Qt::NoPen);
	switch (arrowType) {
		case DConst::NONE: {
			break;
		}
		case DConst::ARROW: {
			QPolygonF arrow;
			arrow << endPoint << arrowP1 << arrowP2;
			painter->drawPolygon(arrow);
			break;
		}
		case DConst::OPEN_ARROW: {
			painter->drawLine(endPoint, arrowP1);
			painter->drawLine(endPoint, arrowP2);
			break;
		}
		case DConst::DOVETAIL_ARROW: {
			QPointF dovetailTip = endPoint - QPointF(cos(angle) * arrowSize,
													 sin(angle) * arrowSize);
			QPointF dovetailP1 = arrowP1 - QPointF(cos(angle) * arrowSize / 2,
												   sin(angle) * arrowSize / 2);
			QPointF dovetailP2 = arrowP2 - QPointF(cos(angle) * arrowSize / 2,
												   sin(angle) * arrowSize / 2);
			QPolygonF dovetail;
			dovetail << endPoint << dovetailP1 << dovetailTip << dovetailP2;
			painter->drawPolygon(dovetail);
			break;
		}
		case DConst::DIAMOND_ARROW: {
			QPointF diamondTip = endPoint - QPointF(cos(angle) * arrowSize,
													sin(angle) * arrowSize);
			QPointF diamondP1 = endPoint - QPointF(cos(angle + DConst::PI / 4) * arrowSize / sqrt(2),
												   sin(angle + DConst::PI / 4) * arrowSize / sqrt(2));
			QPointF diamondP2 = endPoint - QPointF(cos(angle - DConst::PI / 4) * arrowSize / sqrt(2),
												   sin(angle - DConst::PI / 4) * arrowSize / sqrt(2));
			QPolygonF diamond;
			diamond << endPoint << diamondP1 << diamondTip << diamondP2;
			painter->drawPolygon(diamond);
			break;
		}
		case DConst::ROUND_ARROW: {
			painter->drawEllipse(endPoint, arrowSize / 2, arrowSize / 2);
			break;
		}
		default: break;
	}
}

void DLineBase::setBeginPoint(QPointF p)
{
	p = mapFromScene(p);
	sizeToPoint(p, DConst::ST-1);
}

void DLineBase::setEndPoint(QPointF p)
{
	p = mapFromScene(p);
	sizeToPoint(p, DConst::ED-1);
}

int  DLineBase::magType(MagPoint *mag){
    if(endArrowType == DConst::NONE && beginArrowType != DConst::NONE)
        return mag == endMag ? DConst::OUT : DConst::IN;
    if(endArrowType != DConst::NONE && beginArrowType == DConst::NONE)
        return mag == endMag ? DConst::IN : DConst::OUT;
    return DConst::NO_IN_OR_OUT;
}

//==============================================================================

void DLineBase::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DAbstractBase::serialize(out, fa);

	out << beginPoint << endPoint;
	out << (qintptr)beginMag << (qintptr)endMag;
	out << beginArrowType << endArrowType;
}

bool DLineBase::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DAbstractBase::deserialize(in, fa)) return false;

	in >> beginPoint >> endPoint;
	qintptr beginPtr, endPtr; in >> beginPtr >> endPtr;
	linkBegin(Serializer::instance().ptrToMag[beginPtr]);
	linkEnd(Serializer::instance().ptrToMag[endPtr]);

	in >> beginArrowType >> endArrowType;
	updateSizePoint();

	return true;
}
