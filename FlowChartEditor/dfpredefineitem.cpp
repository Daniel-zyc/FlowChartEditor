#include "dfpredefineitem.h"
#include "magpoint.h"

DFPredefineItem::DFPredefineItem(QGraphicsItem *parent)
    : DFPredefineItem(minRectSize, minRectSize, parent) {}

DFPredefineItem::DFPredefineItem(qreal w, qreal h, QGraphicsItem *parent)
    : DShapeBase("", parent)
{
	modis.resize(1);
	for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
	rect = QRectF(-w/2, -h/2, w, h);
	updateAll();
}

void DFPredefineItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    painter->setBrush(brush());
    painter->setPen(pen());
    painter->drawRect(rect);
	painter->drawLine(QPointF(rect.left()+ratio*rect.width(), -0.5*rect.height()),
					  QPointF(rect.left()+ratio*rect.width(), 0.5*rect.height()));
	painter->drawLine(QPointF(rect.right()-ratio*rect.width(), -0.5*rect.height()),
					  QPointF(rect.right()-ratio*rect.width(), 0.5*rect.height()));
}

QRectF DFPredefineItem::sizeRect() const
{
    return rect;
}

QPainterPath DFPredefineItem::shapeNormal() const
{
	QPainterPath pth; pth.addRect(rect); return pth;
}

void DFPredefineItem::updateModiPoint()
{
    modis[0] = {rect.left() + rect.width() * ratio, rect.top()};
}

void DFPredefineItem::updateMagPoint()
{
    (*mags)[0]->setPos({rect.left(), 0});
    (*mags)[1]->setPos({rect.right(), 0});

    (*mags)[2]->setPos({0, rect.top()});
    (*mags)[3]->setPos({0, rect.bottom()});
}

void DFPredefineItem::sizeToRect(QRectF nrect)
{
	rect = nrect; updateAll();
}

void DFPredefineItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(id);
	ratio = (p.x() - rect.left()) / rect.width();
	ratio = qMin(ratio, 0.5);
	ratio = qMax(0.0, ratio);
	updateModiPoint();
}

void DFPredefineItem::updateAll()
{
	updateSizePoint();
	updateMagPoint();
	updateModiPoint();
}

void DFPredefineItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DShapeBase::serialize(out, fa);

	out << rect << ratio;
}

bool DFPredefineItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DShapeBase::deserialize(in, fa)) return false;

	in >> rect >> ratio;
	updateAll();
	return true;
}
