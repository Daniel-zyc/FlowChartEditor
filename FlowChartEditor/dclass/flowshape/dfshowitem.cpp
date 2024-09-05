#include "dfshowitem.h"
#include "../../magpoint.h"

DFShowItem::DFShowItem(QGraphicsItem *parent)
	: DFShowItem(minRectSize, minRectSize, parent) {}

DFShowItem::DFShowItem(qreal w, qreal h, QGraphicsItem *parent)
    : DShapeBase("", parent)
{
	modis.resize(2);
    for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
	rect = QRectF(-w/2, -h/2, w, h);
    updateAll();
}

void DFShowItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    painter->setBrush(brush());
    painter->setPen(pen());
    painter->drawPath(path);
}

QRectF DFShowItem::sizeRect() const
{
    return rect;
}

QPainterPath DFShowItem::shapeNormal() const
{
    return path;
}

void DFShowItem::updateMagPoint()
{
    (*mags)[0]->setPos({rect.left(), 0});
    (*mags)[1]->setPos({rect.right(), 0});
    (*mags)[2]->setPos({0, rect.top()});
    (*mags)[3]->setPos({0, rect.bottom()});
}

void DFShowItem::updatePath()
{
    path.clear();

	path.moveTo(rect.topLeft() + QPointF(rect.width() * ratio1, 0));
	path.lineTo(rect.topRight() - QPointF(rect.width() * ratio2, 0));
	path.arcTo(rect.right() - rect.width() * ratio2 * 2, -rect.height() / 2,
			   rect.width() * ratio2 * 2, rect.height(), 90, -180);
	path.lineTo(rect.bottomLeft() + QPointF(rect.width() * ratio1, 0));
    path.lineTo(-rect.width() / 2, 0);

    path.closeSubpath();
}

void DFShowItem::updateModiPoint()
{
	modis[0] = {rect.left() + rect.width() * ratio1, rect.top()};
	modis[1] = {rect.right() - rect.width() * ratio2, rect.top()};
}

void DFShowItem::sizeToRect(QRectF nrect)
{
    rect = nrect; updateAll();
}

void DFShowItem::modiToPoint(QPointF p, int id)
{
	switch(id)
	{
		case 0:
			ratio1 = (p.x() - rect.left()) / rect.width();
			ratio1 = qMin(0.5, ratio1);
			ratio1 = qMax(0.0, ratio1);
			break;
		case 1:
			ratio2 = (rect.right() - p.x()) / rect.width();
			ratio2 = qMax(0.0, ratio2);
			ratio2 = qMin(0.5, ratio2);
			break;
	}
	updateModiPoint();
	updatePath();
}

void DFShowItem::updateAll()
{
    updateSizePoint();
    updatePath();
    updateMagPoint();
	updateModiPoint();
}

//==============================================================================

void DFShowItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
    DShapeBase::serialize(out, fa);

	out << rect << ratio1 << ratio2;
}

bool DFShowItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
    if(!DShapeBase::deserialize(in, fa)) return false;

	in >> rect >> ratio1 >> ratio2;
    updateAll();
    return true;
}
