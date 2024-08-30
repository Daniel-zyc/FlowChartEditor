#include "dshapebase.h"
#include "dabstractbase.h"
#include "dtextitem.h"

DShapeBase::DShapeBase(QGraphicsItem *parent)
	: DAbstractBase(parent)
{
	maxPointRadius = qMax(maxPointRadius, rotPointRadius);
}

DShapeBase::DShapeBase(const QString &str, QGraphicsItem *parent)
	: DShapeBase(parent)
{
	textItem = new DTextItem(str, this);
}

QRectF DShapeBase::boundingRect() const
{
	qreal r = maxPointRadius;
	qreal top = qMax(r, rotPointMargin + rotPointRadius);
	return sizeRect().normalized().adjusted(-r, -top, r, r);
}

void DShapeBase::paintSelected(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	paintSelectRect(painter, option, widget);
	paintSizePoint(painter, option, widget);
	paintRotPoint(painter, option, widget);
	paintModiPoint(painter, option, widget);
}

void DShapeBase::paintSelectRect(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	painter->setBrush(selectRectBrush);
	painter->setPen(selectRectPen);
	painter->drawRect(sizeRect());
}

void DShapeBase::paintRotPoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	painter->setBrush(selectRectBrush);
	painter->setPen(selectRectPen);

	painter->drawLine(sizes[DConst::T - 1], rotPoint);

	painter->setBrush(rotPointBrush);
	painter->setPen(rotPointPen);

	qreal r = rotPointRadius;
	painter->drawEllipse(rotPoint, r, r);
}

int DShapeBase::checkInterPoint(QPointF p) const
{
	p = mapFromScene(p);
	if(!isSelected()) return DConst::NONE;
	if(checkModiPoint(p) != -1) return DConst::MODI;
	if(checkSizePoint(p) != -1) return DConst::SIZE;
	if(checkRotPoint(p)) return DConst::ROT;
	return DConst::NONE;
}

int DShapeBase::setInterPoint(QPointF p)
{
	p = mapFromScene(p);
	if(!isSelected()) return interactType = DConst::NONE;
	if(setModiPoint(p)) return interactType = DConst::MODI;
	if(setSizePoint(p)) return interactType = DConst::SIZE;
	if(setRotPoint(p)) return interactType = DConst::ROT;
	return interactType = DConst::NONE;
}

void DShapeBase::interToPoint(QPointF p, MagPoint *mp)
{
	Q_UNUSED(mp);
	switch(interactType)
	{
		case DConst::MODI:
			modiToPointPre(mapFromScene(p));
			break;
		case DConst::SIZE:
			sizeToPointPre(mapFromScene(p));
			break;
		case DConst::ROT:
			QPointF c = mapToScene({0, 0});
			rotToPoint(p - c);
			break;
	}
}

bool DShapeBase::checkRotPoint(QPointF p) const
{
	qreal r = rotPointRadius;
	QPointF rp = rotPoint;
	return QRectF(rp.x()-r, rp.y()-r, 2*r, 2*r).contains(p);
}

bool DShapeBase::setRotPoint(QPointF p)
{
	return checkSizePoint(p);
}

void DShapeBase::sizeToPoint(QPointF p, int id, MagPoint *mp)
{
	Q_UNUSED(mp);
	QRectF nrect = getResizeRect(p, id);
	QPointF cent = mapToParent(nrect.center());
	nrect.moveCenter({0, 0});

	prepareGeometryChange();
	sizeToRect(nrect);
	sizeRectUpdated();
	setPos(cent);
	updateAllLinkLines();
	update();
}

void DShapeBase::rotToPoint(QPointF p)
{
	qreal deg = radToDeg(atan2(p.x(), -p.y()));
	setRotation(deg);
}

void DShapeBase::sizeRectUpdated()
{
	QRectF rc = sizeRect();
	qreal minx = rc.left(), maxx = rc.right(), midx = rc.center().x();
	qreal miny = rc.top(), maxy = rc.bottom(), midy = rc.center().y();
	
	sizes.resize(8);
	sizes[DConst::T - 1] = {midx, miny};
	sizes[DConst::B - 1] = {midx, maxy};
	sizes[DConst::L - 1] = {minx, midy};
	sizes[DConst::R - 1] = {maxx, midy};
	sizes[DConst::TL - 1] = {minx, miny};
	sizes[DConst::TR - 1] = {maxx, miny};
	sizes[DConst::BL - 1] = {minx, maxy};
	sizes[DConst::BR - 1] = {maxx, maxy};

	rotPoint = {midx, miny - rotPointMargin};
}

QPainterPath DShapeBase::shapeSelected() const
{
	QPainterPath pth;
	pth.addRect(boundingRect());
	return pth;
}

QPainterPath DShapeBase::shapeShowMaged() const
{
	QPainterPath pth;
	qreal r = magPointCollideRadius;
	pth.addRect(sizeRect().adjusted(-r, -r, r, r));
	return pth;
}

QVariant DShapeBase::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if(change == QGraphicsItem::ItemPositionHasChanged
	   || change == QGraphicsItem::ItemTransformHasChanged
	   || change == QGraphicsItem::ItemRotationHasChanged
	   || change == QGraphicsItem::ItemScaleHasChanged)
		updateAllLinkLines();

	return value;
}

QRectF DShapeBase::getResizeRect(const QPointF &p, int id)
{
    QRectF nrc = this->sizeRect();
	qreal x = p.x(), y = p.y();
	qreal r = sizePointRadius;

	auto resizeT = [&](){ if(y < nrc.bottom() - 4*r) nrc.setTop(y); };
	auto resizeB = [&](){ if(y > nrc.top() + 4*r) nrc.setBottom(y); };
	auto resizeL = [&](){ if(x < nrc.right() - 4*r) nrc.setLeft(x); };
	auto resizeR = [&](){ if(x > nrc.left() + 4*r) nrc.setRight(x); };

	switch(id + 1)
	{
		case DConst::T: resizeT(); break;
		case DConst::B: resizeB(); break;
		case DConst::L: resizeL(); break;
		case DConst::R: resizeR(); break;
		case DConst::TL: resizeT(); resizeL(); break;
		case DConst::TR: resizeT(); resizeR(); break;
		case DConst::BL: resizeB(); resizeL(); break;
		case DConst::BR: resizeB(); resizeR(); break;
		case DConst::NONE: break;
	}
	return nrc;
}

//===========================================
void DShapeBase::serialize(QDataStream &out) const{
    DAbstractBase::serialize(out);
}

void DShapeBase::deserialize(QDataStream &in){
    DAbstractBase::deserialize(in);
}
