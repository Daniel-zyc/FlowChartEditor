#include "dfnodeitem.h"
#include "../../magpoint.h"

DFNodeItem::DFNodeItem(QGraphicsItem *parent)
	: DFNodeItem(minRectSize, minRectSize, parent) {}

DFNodeItem::DFNodeItem(qreal w, qreal h, QGraphicsItem *parent)
	: DShapeBase("", parent)
{
	for(int i = 0; i < 8; i++) mags->push_back(new MagPoint(this));
	rect = QRectF(-w/2, -h/2, w, h);
	updateAll();
}

void DFNodeItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);

	painter->setBrush(brush());
	painter->setPen(pen());
	painter->drawEllipse(rect);
}

QRectF DFNodeItem::sizeRect() const
{
	return rect;
}

QPainterPath DFNodeItem::shapeNormal() const
{
	QPainterPath pth; pth.addEllipse(rect); return pth;
}

void DFNodeItem::updateMagPoint()
{
	(*mags)[0]->setPos({rect.left(), 0});
	(*mags)[1]->setPos({rect.right(), 0});
	(*mags)[2]->setPos({0, rect.top()});
	(*mags)[3]->setPos({0, rect.bottom()});

	qreal rx = rect.width() / 2, ry = rect.height() / 2;
	(*mags)[4]->setPos({rx * cos(DTool::degToRad(45)), ry * sin(DTool::degToRad(45))});
	(*mags)[5]->setPos({rx * cos(DTool::degToRad(135)), ry * sin(DTool::degToRad(45))});
	(*mags)[6]->setPos({rx * cos(DTool::degToRad(-135)), ry * sin(DTool::degToRad(-135))});
	(*mags)[7]->setPos({rx * cos(DTool::degToRad(-45)), ry * sin(DTool::degToRad(-45))});
}

void DFNodeItem::sizeToRect(QRectF nrect)
{
	rect = nrect; updateAll();
}

void DFNodeItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(p); Q_UNUSED(id); return;
}

void DFNodeItem::updateAll()
{
	updateSizePoint();
	updateMagPoint();
}

//======================================

void DFNodeItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DShapeBase::serialize(out, fa);

	out << rect;
}

bool DFNodeItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DShapeBase::deserialize(in, fa)) return false;

	in >> rect;
	updateAll();
	return true;
}
