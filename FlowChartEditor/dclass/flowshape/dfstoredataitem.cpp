#include "dfstoredataitem.h"
#include <QGraphicsItem>
#include <QPainter>
#include <QPainterPath>
#include "../../magpoint.h"

DFStoreDataItem::DFStoreDataItem(QGraphicsItem *parent)
    : DFStoreDataItem(minRectSize, minRectSize, parent) {}

DFStoreDataItem::DFStoreDataItem(qreal w, qreal h, QGraphicsItem *parent)
    : DShapeBase("", parent)
{
	modis.resize(1);
    for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
	rect = QRectF(-w/2, -h/2, w, h);
	ratio = 0.14;
    updateAll();
}

void DFStoreDataItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    painter->setBrush(brush());
    painter->setPen(pen());
    painter->drawPath(path);
}

QRectF DFStoreDataItem::sizeRect() const
{
    return rect;
}

QPainterPath DFStoreDataItem::shapeNormal() const
{
    return path;
}

void DFStoreDataItem::updateMagPoint()
{
    (*mags)[0]->setPos({rect.left(), 0});
	(*mags)[1]->setPos({rect.right() - ratio * rect.width(), 0});

    (*mags)[2]->setPos({0, rect.top()});
    (*mags)[3]->setPos({0, rect.bottom()});
}

void DFStoreDataItem::updateModiPoint()
{
	modis[0] = {rect.left() + ratio * rect.width(), rect.top()};
}

void DFStoreDataItem::updatePath()
{
    path.clear();

    qreal rectWidth = rect.width() / 2;
    qreal rectHeight = rect.height();
    qreal ellipseRadiusX = rectWidth;
    qreal ellipseRadiusY = rectHeight / 2;

	path.moveTo(rect.left()+ratio*rect.width(),rect.top());
    // 绘制左半部分椭圆
	path.arcTo(rect.left(),rect.top(),ratio*2*rect.width(),rect.height(),90,180);
    path.lineTo(rect.right(), rect.bottom());
	path.moveTo(rect.left()+ratio*rect.width(),rect.top());
    path.lineTo(rect.right(), rect.top());
    // 绘制右半部分椭圆
	path.arcTo(rect.right()-ratio*rect.width(),rect.top(),ratio*2*rect.width(),rect.height(),90,180);
}

void DFStoreDataItem::sizeToRect(QRectF nrect)
{
    rect = nrect; updateAll();
}

void DFStoreDataItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(id);
	qreal nx = p.x();
	nx = qMin(nx, 0.0); nx = qMax(nx, rect.left());
	ratio = (nx - rect.left()) / rect.width();
	updateModiPoint();
	updatePath();
	updateMagPoint();
}

void DFStoreDataItem::updateAll()
{
    updateSizePoint();
    updatePath();
    updateMagPoint();
	updateModiPoint();
}

void DFStoreDataItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
    DShapeBase::serialize(out, fa);

	out << rect << ratio;
}

bool DFStoreDataItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
    if(!DShapeBase::deserialize(in, fa)) return false;

	in >> rect >> ratio;
    updateAll();
    return true;
}
