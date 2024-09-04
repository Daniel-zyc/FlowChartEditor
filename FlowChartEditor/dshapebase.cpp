#include "dshapebase.h"
#include "dabstractbase.h"
#include "dtextitem.h"
#include "magpoint.h"

DShapeBase::DShapeBase(QGraphicsItem *parent)
	: DAbstractBase(parent) {}

DShapeBase::DShapeBase(const QString &text, QGraphicsItem *parent)
	: DShapeBase(parent)
{
	textItem = new DTextItem(text, this);
	textItem->deleteMagPoint();

}

QRectF DShapeBase::boundingRect() const
{
	qreal r = maxBorderRadius;
	qreal top = qMax(r, rotPointMargin + rotPointRadius);
	return sizeRect().normalized().adjusted(-r, -top, r, r);
}

void DShapeBase::paintSelected(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	paintSelectRect(painter, option, widget);
	DAbstractBase::paintSelected(painter, option, widget);
	if(isRotateable) paintRotPoint(painter, option, widget);
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
	if(isRotateable && checkRotPoint(p)) return DConst::ROT;
	return DConst::NONE;
}

int DShapeBase::setInterPoint(QPointF p)
{
	p = mapFromScene(p);
	if(!isSelected()) return interactType = DConst::NONE;
	if(setModiPoint(p)) return interactType = DConst::MODI;
	if(setSizePoint(p)) return interactType = DConst::SIZE;
	if(isRotateable && setRotPoint(p)) return interactType = DConst::ROT;
	return interactType = DConst::NONE;
}

void DShapeBase::interToPoint(QPointF p, MagPoint *mp)
{
	Q_UNUSED(mp);
	if(interactType == DConst::NONE) return;

	prepareGeometryChange();
	switch(interactType)
	{
		case DConst::MODI:
			modiToPointPre(mapFromScene(p));
			break;
		case DConst::SIZE:
			sizeToPointPre(mapFromScene(p));
			break;
		case DConst::ROT:
			rotToPoint(p);
			break;
	}
	updateAllLinkLines();
	update();
}

void DShapeBase::setInsertItem()
{
	interactType = DConst::SIZE;
	sizePointId = DConst::BR - 1;
}

bool DShapeBase::checkRotPoint(QPointF p) const
{
	qreal r = rotPointRadius; QPointF rp = rotPoint;
	return DTool::inCircle(rp, r, p);
}

bool DShapeBase::setRotPoint(QPointF p)
{
	return checkRotPoint(p);
}

void DShapeBase::sizeToPoint(QPointF p, int id, MagPoint *mp)
{
	Q_UNUSED(mp);
	QRectF nrect = getResizeRect(p, id);
	QPointF cent = mapToParent(nrect.center());
	nrect.moveCenter({0, 0});

	sizeToRect(nrect);
	updateSizePoint();
	setPos(cent);
}

void DShapeBase::rotToPoint(QPointF p)
{
	p = mapToParent(mapFromScene(p)) - mapToParent({0, 0});
	qreal deg = DTool::radToDeg(atan2(p.x(), -p.y()));
	setRotation(deg);
}

void DShapeBase::updateSizePoint()
{
	QRectF rc = sizeRect();
	qreal minx = rc.left(), maxx = rc.right();
	qreal miny = rc.top(), maxy = rc.bottom();
	
	sizes.resize(8);
	sizes[DConst::T - 1] = {0, miny};
	sizes[DConst::B - 1] = {0, maxy};
	sizes[DConst::L - 1] = {minx, 0};
	sizes[DConst::R - 1] = {maxx, 0};
	sizes[DConst::TL - 1] = {minx, miny};
	sizes[DConst::TR - 1] = {maxx, miny};
	sizes[DConst::BL - 1] = {minx, maxy};
	sizes[DConst::BR - 1] = {maxx, maxy};

	rotPoint = {0, miny - rotPointMargin};
}

QPainterPath DShapeBase::shapeSelected() const
{
	QPainterPath pth = DAbstractBase::shapeSelected();
	qreal r = rotPointRadius; pth.addEllipse(rotPoint, r, r);
	pth.addRect(sizeRect());
	return pth;
}

QVariant DShapeBase::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if(change == QGraphicsItem::ItemPositionHasChanged
	   || change == QGraphicsItem::ItemRotationHasChanged
	   || change == QGraphicsItem::ItemScaleHasChanged)
		updateAllLinkLines();
	return value;
}

QRectF DShapeBase::getResizeRect(const QPointF &p, int id)
{
    QRectF nrc = this->sizeRect();
	qreal x = p.x(), y = p.y();

	auto resizeT = [&](){ if(y < nrc.bottom() - minRectSize) nrc.setTop(y); };
	auto resizeB = [&](){ if(y > nrc.top() + minRectSize) nrc.setBottom(y); };
	auto resizeL = [&](){ if(x < nrc.right() - minRectSize) nrc.setLeft(x); };
	auto resizeR = [&](){ if(x > nrc.left() + minRectSize) nrc.setRight(x); };

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

bool DShapeBase::isTextEmpty(){
    if(textItem == nullptr) return false;
    return textItem->textBase.toPlainText() == "";
}

//==============================================================================

void DShapeBase::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DAbstractBase::serialize(out, fa);

	if(mags == nullptr) out << (quint32)0;
	else{
		out << (quint32)mags->size();
		for(MagPoint *magPoint : *mags) magPoint->serialize(out);
	}

	out << (qintptr)textItem;
	if(textItem) textItem->serialize(out, this);
}

bool DShapeBase::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DAbstractBase::deserialize(in, fa)) return false;

	quint32 magPointCount; in >> magPointCount;
	for(quint32 i = 0; i < magPointCount; i++)
		(*mags)[i]->deserialize(in, this);

	qintptr item; in >> item;
	if(item) textItem->deserialize(in, this);
	else
	{
		delete textItem;
		textItem = nullptr;
	}

	return true;
}
