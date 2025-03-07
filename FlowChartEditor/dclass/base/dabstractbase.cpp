#include "../../magpoint.h"
#include "dabstractbase.h"

DAbstractBase::DAbstractBase(QGraphicsItem *parent)
	: QAbstractGraphicsShapeItem(parent)
{
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable
			 | QGraphicsItem::ItemSendsGeometryChanges);
	mags = new QList<MagPoint*>();
}

DAbstractBase::~DAbstractBase()
{
	if(mags) for(MagPoint* mag : *mags) delete mag;
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

void DAbstractBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	paintShape(painter, option, widget);
	if(isSelected()) paintSelected(painter, option, widget);
	if(showMagPoint) paintMagPoint(painter, option, widget);
}

//==============================================================================

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

//==============================================================================

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

//==============================================================================

void DAbstractBase::setShowMagPoint(bool show)
{
	showMagPoint = show;
	update();
}

bool DAbstractBase::checkMagPoint(QPointF p) const
{
	p = mapFromScene(p);
	qreal r = magPointCollideRadius;
	for(int i = 0; i < mags->size(); i++)
		if(DTool::inCircle((*mags)[i]->pos, r, p))
			return true;
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

//==============================================================================

int DAbstractBase::checkModiPoint(QPointF p) const
{
	qreal r = modiPointRadius;
	for(int i = 0; i < modis.size(); i++) if(DTool::inCircle(modis[i], r, p))
		return i;
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
	for(int i = 0; i < sizes.size(); i++) if(DTool::inCircle(sizes[i], r, p))
		return i;
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

void DAbstractBase::modiToPointPre(QPointF p)
{
	if(modiPointId == -1) return;
	modiToPoint(p, modiPointId);
}

void DAbstractBase::sizeToPointPre(QPointF p, MagPoint *mp)
{
	if(sizePointId == -1) return;
	sizeToPoint(p, sizePointId, mp);
}

//==============================================================================

void DAbstractBase::setScale(qreal scl)
{
	if(!isScaleable) return;
	QGraphicsItem::setScale(scl);
}

void DAbstractBase::setRotation(qreal deg)
{
	if(!isRotateable) return;
	QGraphicsItem::setRotation(deg);
}

void DAbstractBase::setRotateable(bool state)
{
	if(state == isRotateable) return;
	prepareGeometryChange();
	isRotateable = state;
	update();
}

void DAbstractBase::setScaleable(bool state)
{
	if(state == isScaleable) return;
	isScaleable = state;
}

void DAbstractBase::updateAllLinkLines()
{
	for(MagPoint* mag : *mags) mag->updateAllLinkLines();
}

void DAbstractBase::unLinkAllLines()
{
	for(MagPoint* mag : *mags) mag->unlinkAllLines();
}

std::tuple<int,int,int> DAbstractBase::getLinedArrowType()
{
	if(mags == nullptr) return std::make_tuple(0,0,0);
	int in = 0, out = 0, none = 0;
	for(MagPoint * mag : *mags){
		auto result = mag->getLinkedLineArrowType();
		in += std::get<0>(result);
		out += std::get<1>(result);
		none += std::get<2>(result);
	}
	return std::make_tuple(in,out,none);
}

QVariant DAbstractBase::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if(change == QGraphicsItem::ItemPositionHasChanged
		|| change == QGraphicsItem::ItemRotationHasChanged
		|| change == QGraphicsItem::ItemScaleHasChanged)
	{
		SHOT_STATE = DConst::CHANGED;
	}
	return QGraphicsItem::itemChange(change, value);
}

//==============================================================================

void DAbstractBase::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	if(fa != nullptr || parentItem() == nullptr)
		out << pos() << rotation() << scale() << zValue();
	else
		out << scenePos() << rotation() + parentItem()->rotation()
			<< scale() * parentItem()->scale() << parentItem()->zValue();

	out << brush() << pen();
}

bool DAbstractBase::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(fa) setParentItem(fa);

	// 关闭发送变化的信号，避免进行由 itemChange引发的多余调整
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);

	QPointF pos; in >> pos; setPos(pos);
	qreal rot; in >> rot; QGraphicsItem::setRotation(rot);
	qreal scl; in >> scl; QGraphicsItem::setScale(scl);
	qreal zval; in >> zval; setZValue(zval + TOTAL_MAX_Z_VALUE );
	QBrush qb; in >> qb; setBrush(qb);
	QPen qp; in >> qp; setPen(qp);

	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

	return true;
}
