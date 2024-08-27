#include "dshapebase.h"

QBrush DShapeBase::selectedRectBrush = QBrush(Qt::transparent, Qt::SolidPattern);
QPen DShapeBase::selectedRectPen = QPen(Qt::black, 1, Qt::DashLine);
QBrush DShapeBase::sizePointBrush = QBrush(Qt::white, Qt::SolidPattern);
QPen DShapeBase::sizePointPen = QPen(Qt::black, 1, Qt::SolidLine);
QBrush DShapeBase::magPointBrush = QBrush(Qt::gray, Qt::SolidPattern);
QPen DShapeBase::magPointPen = QPen(Qt::gray, 1, Qt::SolidLine);
QBrush DShapeBase::modiPointBrush = QBrush(Qt::yellow, Qt::SolidPattern);
QPen DShapeBase::modiPointPen = QPen(Qt::black, 1, Qt::SolidLine);

DShapeBase::DShapeBase(QGraphicsItem *parent)
	:QAbstractGraphicsShapeItem(parent), textItem(this)
{
	maxPointRadius = qMax(sizePointRadius, qMax(magPointRadius, modiPointRadius));
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable
			 | QGraphicsItem::ItemSendsGeometryChanges);
	textItem.setPlainText("Hello World!");
	showMagPoint = true;
}

QRectF DShapeBase::boundingRect() const
{
	qreal r = maxPointRadius;
	return sizeRect().adjusted(-r, -r, r, r);
}

QPainterPath DShapeBase::shapeSelected() const
{
	QPainterPath pth;
	pth.addRect(boundingRect());
	return pth;
}

QPainterPath DShapeBase::shape() const
{
	if(isSelected() || showMagPoint) return shapeSelected();
	else return shapeNormal();
}

void DShapeBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	paintShape(painter, option, widget);
	if(isSelected())
	{
		paintSelectRect(painter, option, widget);
		paintSizePoint(painter, option, widget);
		paintModifyPoint(painter, option, widget);
	}
	if(showMagPoint) paintMagPoint(painter, option, widget);
}

void DShapeBase::paintSelectRect(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	painter->setBrush(selectedRectBrush);
	painter->setPen(selectedRectPen);
	painter->drawRect(sizeRect());
}

void DShapeBase::paintSizePoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	painter->setBrush(sizePointBrush);
	painter->setPen(sizePointPen);

	QRectF rc = sizeRect();
	qreal minx = rc.left(), maxx = rc.right(), midx = rc.center().x();
	qreal miny = rc.top(), maxy = rc.bottom(), midy = rc.center().y();
	qreal r = sizePointRadius;

	painter->drawEllipse({midx, maxy}, r, r);
	painter->drawEllipse({midx, miny}, r, r);
	painter->drawEllipse({maxx, midy}, r, r);
	painter->drawEllipse({minx, midy}, r, r);
	painter->drawEllipse({maxx, maxy}, r, r);
	painter->drawEllipse({minx, maxy}, r, r);
	painter->drawEllipse({maxx, miny}, r, r);
	painter->drawEllipse({minx, miny}, r, r);
}

void DShapeBase::paintMagPoint(QPainter *painter, const QStyleOptionGraphicsItem *option,
							   QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	painter->setBrush(magPointBrush);
	painter->setPen(magPointPen);

	qreal r = magPointRadius;
	for(const MagPoint& mag : mags) painter->drawEllipse({mag.x, mag.y}, r, r);
}

void DShapeBase::paintModifyPoint(QPainter *painter, const QStyleOptionGraphicsItem *option,
								  QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	painter->setBrush(modiPointBrush);
	painter->setPen(modiPointPen);

	qreal r = modiPointRadius;
	for(const QPointF& modi : modis) painter->drawEllipse(modi, r, r);
}

bool DShapeBase::checkModiPoint(QPointF p, bool isItemCord) const
{
	if(!isItemCord) p = mapFromScene(p);
	qreal r = modiPointRadius;
	for(int i = 0; i < modis.size(); i++)
	{
		const QPointF& mp = modis[i];
		if(QRectF(mp.x() - r, mp.y() - r, 2*r, 2*r).contains(p))
			return true;
	}
	return false;
}

bool DShapeBase::setModiPoint(QPointF p, bool isItemCord)
{
	if(!isItemCord) p = mapFromScene(p);
	qreal r = modiPointRadius;
	modiPointId = -1;
	for(int i = 0; i < modis.size(); i++)
	{
		const QPointF& mp = modis[i];
		if(QRectF(mp.x() - r, mp.y() - r, 2*r, 2*r).contains(p))
		{
			modiPointId = i;
			break;
		}
	}
	return modiPointId != -1;
}

bool DShapeBase::checkSizePoint(QPointF p, bool isItemCord) const
{
	if(!isItemCord) p = mapFromScene(p);

	QRectF rc = sizeRect();
	qreal minx = rc.left(), maxx = rc.right(), midx = rc.center().x();
	qreal miny = rc.top(), maxy = rc.bottom(), midy = rc.center().y();
	qreal r = sizePointRadius;

	if(QRectF(minx-r, miny-r, 2*r, 2*r).contains(p)) return true;
	if(QRectF(maxx-r, miny-r, 2*r, 2*r).contains(p)) return true;
	if(QRectF(minx-r, maxy-r, 2*r, 2*r).contains(p)) return true;
	if(QRectF(maxx-r, maxy-r, 2*r, 2*r).contains(p)) return true;

	if(QRectF(minx-r, midy-r, 2*r, 2*r).contains(p)) return true;
	if(QRectF(maxx-r, midy-r, 2*r, 2*r).contains(p)) return true;
	if(QRectF(midx-r, miny-r, 2*r, 2*r).contains(p)) return true;
	if(QRectF(midx-r, maxy-r, 2*r, 2*r).contains(p)) return true;

	return false;
}

bool DShapeBase::setSizePoint(QPointF p, bool isItemCord)
{
	if(!isItemCord) p = mapFromScene(p);

	QRectF rc = sizeRect();
	qreal minx = rc.left(), maxx = rc.right(), midx = rc.center().x();
	qreal miny = rc.top(), maxy = rc.bottom(), midy = rc.center().y();
	qreal r = sizePointRadius;

	resizeOrient = ResizeOrient::NONE;
	if(QRectF(minx-r, miny-r, 2*r, 2*r).contains(p)) resizeOrient = ResizeOrient::TL;
	if(QRectF(maxx-r, miny-r, 2*r, 2*r).contains(p)) resizeOrient = ResizeOrient::TR;
	if(QRectF(minx-r, maxy-r, 2*r, 2*r).contains(p)) resizeOrient = ResizeOrient::BL;
	if(QRectF(maxx-r, maxy-r, 2*r, 2*r).contains(p)) resizeOrient = ResizeOrient::BR;

	if(QRectF(minx-r, midy-r, 2*r, 2*r).contains(p)) resizeOrient = ResizeOrient::L;
	if(QRectF(maxx-r, midy-r, 2*r, 2*r).contains(p)) resizeOrient = ResizeOrient::R;
	if(QRectF(midx-r, miny-r, 2*r, 2*r).contains(p)) resizeOrient = ResizeOrient::T;
	if(QRectF(midx-r, maxy-r, 2*r, 2*r).contains(p)) resizeOrient = ResizeOrient::B;

	return resizeOrient != ResizeOrient::NONE;
}

bool DShapeBase::checkMagPoint(QPointF p, bool isItemCord) const
{
	if(!isItemCord) p = mapFromScene(p);
	qDebug() << "check p: " << p;
	qreal r = magPointRadius;
	for(int i = 0; i < mags.size(); i++)
	{
		const MagPoint& mp = mags[i];
		qDebug() << mp.x << " " << mp.y;
		if(QRectF(mp.x - r, mp.y - r, 2*r, 2*r).contains(p))
			return true;
	}
	return false;
}

void DShapeBase::setShowMagPoint(bool show)
{
	showMagPoint = show;
}

MagPoint* DShapeBase::getMagPoint(QPointF p, bool isItemCord)
{
	if(!isItemCord) p = mapFromScene(p);
	qreal r = magPointRadius;
	for(int i = 0; i < mags.size(); i++)
	{
		const MagPoint& mp = mags[i];
		if(QRectF(mp.x - r, mp.y - r, 2*r, 2*r).contains(p))
			return &mags[i];
	}
	return nullptr;
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

void DShapeBase::resizeToPoint(QPointF p, bool isItemCord)
{
	if(!isItemCord) p = mapFromScene(p);
	QRectF nrc = getResizeRect(p);
	QPointF cent = mapToScene(nrc.center());
	nrc.moveCenter({0, 0});

	prepareGeometryChange();
	resizeToRect(nrc);
	setPos(cent);
	updateAllLinkLines();
	update();
}

void DShapeBase::modiToPoint(QPointF p, bool isItemCord)
{
	if(!isItemCord) p = mapFromScene(p);
	prepareGeometryChange();
	modifyToPoint(p, modiPointId);
	updateAllLinkLines();
	update();
}

QVariant DShapeBase::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if(change == QGraphicsItem::ItemPositionHasChanged
	   || change == QGraphicsItem::ItemScaleHasChanged
	   || change == QGraphicsItem::ItemRotationHasChanged)
	{
		updateAllLinkLines();
	}

	return value;
}

void DShapeBase::updateAllLinkLines()
{
	for(const MagPoint& mag : mags) mag.updateLines();
}

