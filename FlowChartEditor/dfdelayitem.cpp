#include "dfdelayitem.h"
#include <QGraphicsItem>
#include <QPainter>
#include <QPainterPath>
#include "magpoint.h"

DFDelayItem::DFDelayItem(QGraphicsItem *parent)
    : DFDelayItem(minRectSize, minRectSize, parent) {}

DFDelayItem::DFDelayItem(qreal w, qreal h, QGraphicsItem *parent)
    : DShapeBase("", parent)
{
    for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
    rect = QRectF(-w/2, -h/2, w, h);
    updateAll();
}

void DFDelayItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    painter->setBrush(brush());
    painter->setPen(pen());
    painter->drawPath(path);
}

QRectF DFDelayItem::sizeRect() const
{
    return rect;
}

QPainterPath DFDelayItem::shapeNormal() const
{
    return path;
}

void DFDelayItem::updateMagPoint()
{
    (*mags)[0]->setPos({rect.left(), 0});
    (*mags)[1]->setPos({rect.right(), 0});

    (*mags)[2]->setPos({0, rect.top()});
    (*mags)[3]->setPos({0, rect.bottom()});
}

void DFDelayItem::updatePath()
{
    path.clear();

    qreal rectWidth = rect.width() / 2;
    qreal rectHeight = rect.height();
    qreal ellipseRadiusX = rectWidth;
    qreal ellipseRadiusY = rectHeight / 2;

    // 绘制右半部分椭圆
    QPointF ellipseCenter = rect.topRight() + QPointF(-ellipseRadiusX, rect.height()/2);
    path.addEllipse(ellipseCenter, ellipseRadiusX, ellipseRadiusY); // 绘制半个椭圆

    // 合并矩形和椭圆的路径
    QPainterPath rectPath;
    rectPath.addRect(QRectF(rect.topLeft(), QSizeF(rectWidth, rectHeight)));
    path = rectPath.united(path);
}

void DFDelayItem::sizeToRect(QRectF nrect)
{
    rect = nrect; updateAll();
}

void DFDelayItem::modiToPoint(QPointF p, int id)
{
    Q_UNUSED(p); Q_UNUSED(id); return;
}

void DFDelayItem::updateAll()
{
    updateSizePoint();
    updatePath();
    updateMagPoint();
}

void DFDelayItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DShapeBase::serialize(out, fa);

	out << rect;
}

bool DFDelayItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DShapeBase::deserialize(in, fa)) return false;

	in >> rect;
	updateAll();
	return true;
}
