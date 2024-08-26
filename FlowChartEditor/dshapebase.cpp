#include "dshapebase.h"

DShapeBase::DShapeBase(QGraphicsItem *parent)
	:QAbstractGraphicsShapeItem(parent), sizePointRadius(5),
	activeInteractType(InteractPoint::NONE), resizeOrient(ResizeOrient::NONE)
{
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable
			 | QGraphicsItem::ItemSendsGeometryChanges);
}

QRectF DShapeBase::boundingRect() const
{
	qreal r = sizePointRadius;
	return sizeRect().adjusted(-r, -r, r, r);
}

QPainterPath DShapeBase::shapeNormal() const
{
	QPainterPath pth;
	pth.addRect(sizeRect());
	return pth;
}

QPainterPath DShapeBase::shapeSelected() const
{
	QPainterPath pth;
	pth.addRect(boundingRect());
	return pth;
}

QPainterPath DShapeBase::shape() const
{
	if(isSelected()) return shapeSelected();
	else return shapeNormal();
}

void DShapeBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	paintShape(painter, option, widget);
	if(option->state & QStyle::State_Selected)
	{
		paintSelectRect(painter, option, widget);
		paintSizePoint(painter, option, widget);
		paintModifyPoint(painter, option, widget);
		paintMagPoint(painter, option, widget);
	}
}

void DShapeBase::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	painter->setBrush(brush());
	painter->setPen(pen());
	painter->drawRect(sizeRect());
}

void DShapeBase::paintSelectRect(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	painter->setBrush(QBrush(Qt::transparent));
	QPen qpen(Qt::black); qpen.setStyle(Qt::DashLine);
	painter->setPen(qpen);
	painter->drawRect(sizeRect());
}

void DShapeBase::paintSizePoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	QRectF rc = sizeRect();
	qreal minx = rc.left(), maxx = rc.right(), midx = rc.center().x();
	qreal miny = rc.top(), maxy = rc.bottom(), midy = rc.center().y();
	qreal r = sizePointRadius;

	painter->setBrush(QBrush(Qt::white));
	painter->setPen(QPen(Qt::black));

	painter->drawEllipse(QPointF(minx, miny), r, r);
	painter->drawEllipse(QPointF(maxx, miny), r, r);
	painter->drawEllipse(QPointF(minx, maxy), r, r);
	painter->drawEllipse(QPointF(maxx, maxy), r, r);
	painter->drawEllipse(QPointF(midx, miny), r, r);
	painter->drawEllipse(QPointF(midx, maxy), r, r);
	painter->drawEllipse(QPointF(minx, midy), r, r);
	painter->drawEllipse(QPointF(maxx, midy), r, r);
}

void DShapeBase::paintMagPoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

void DShapeBase::paintModifyPoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}

bool DShapeBase::checkInteractPoint(const QPointF &p)
{
	return checkSizePoint(p) != ResizeOrient::NONE;
}

ResizeOrient DShapeBase::checkSizePoint(const QPointF &p)
{
	QRectF rc = sizeRect();
	qreal minx = rc.left(), maxx = rc.right(), midx = rc.center().x();
	qreal miny = rc.top(), maxy = rc.bottom(), midy = rc.center().y();
	qreal r = sizePointRadius;

	if(QRectF(minx-r, miny-r, 2*r, 2*r).contains(p)) return resizeOrient = ResizeOrient::TL;
	if(QRectF(maxx-r, miny-r, 2*r, 2*r).contains(p)) return resizeOrient = ResizeOrient::TR;
	if(QRectF(minx-r, maxy-r, 2*r, 2*r).contains(p)) return resizeOrient = ResizeOrient::BL;
	if(QRectF(maxx-r, maxy-r, 2*r, 2*r).contains(p)) return resizeOrient = ResizeOrient::BR;

	if(QRectF(minx-r, midy-r, 2*r, 2*r).contains(p)) return resizeOrient = ResizeOrient::L;
	if(QRectF(maxx-r, midy-r, 2*r, 2*r).contains(p)) return resizeOrient = ResizeOrient::R;
	if(QRectF(midx-r, miny-r, 2*r, 2*r).contains(p)) return resizeOrient = ResizeOrient::T;
	if(QRectF(midx-r, maxy-r, 2*r, 2*r).contains(p)) return resizeOrient = ResizeOrient::B;

	return resizeOrient = ResizeOrient::NONE;
}

void DShapeBase::setActiveInteractPoint(const QPointF &p)
{
	if((int)checkSizePoint(p))
	{
		activeInteractType = InteractPoint::SIZE;
		setActiveSizePoint(checkSizePoint(p));
		return;
	}

	activeInteractType = InteractPoint::NONE;
}

void DShapeBase::setActiveSizePoint(ResizeOrient orient)
{
	resizeOrient = orient;
}

QRectF DShapeBase::getResizeRect(const QPointF &p)
{
	QRectF nrc = this->sizeRect();
	qreal x = p.x(), y = p.y();
	qreal r = sizePointRadius;

	auto resizeT = [&](){ if(y < nrc.bottom() - 4*r) nrc.setTop(y); };
	auto resizeB = [&](){ if(y > nrc.top() + 4*r) nrc.setBottom(y); };
	auto resizeL = [&](){ if(x < nrc.right() - 4*r) nrc.setLeft(x); };
	auto resizeR = [&](){ if(x > nrc.left() + 4*r) nrc.setRight(x); };

	switch(resizeOrient)
	{
		case ResizeOrient::T: resizeT(); break;
		case ResizeOrient::B: resizeB(); break;
		case ResizeOrient::L: resizeL(); break;
		case ResizeOrient::R: resizeR(); break;
		case ResizeOrient::TL: resizeT(); resizeL(); break;
		case ResizeOrient::TR: resizeT(); resizeR(); break;
		case ResizeOrient::BL: resizeB(); resizeL(); break;
		case ResizeOrient::BR: resizeB(); resizeR(); break;
		case ResizeOrient::NONE: break;
	}
	return nrc;
}

void DShapeBase::resizeToPoint(const QPointF &p)
{
	QRectF nrc = getResizeRect(p);
	QPointF cent = mapToScene(nrc.center());
	nrc.moveCenter({0, 0});

	prepareGeometryChange();
	resizeToRect(nrc);
	setPos(cent);
	update();
}

QVariant DShapeBase::itemChange(GraphicsItemChange change, const QVariant &value)
{
	// qDebug() << change;
	// if(change == QGraphicsItem::ItemPositionHasChanged)
	// {
		qDebug() << "changed";
		updateLinkedArrow();
	// }

	return value;
}

void DShapeBase::updateLinkedArrow()
{
	for(DLineItem *item : arrows) item->updatePosition();
}

