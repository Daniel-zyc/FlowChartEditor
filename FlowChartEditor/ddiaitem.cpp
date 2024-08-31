#include "ddiaitem.h"
#include "magpoint.h"

DDiaItem::DDiaItem(QGraphicsItem *parent)
    : DShapeBase(parent) {}

DDiaItem::DDiaItem(qreal w, qreal h, QGraphicsItem *parent)
    : DDiaItem(parent)
{
    mags->push_back(new MagPoint(this));
    mags->push_back(new MagPoint(this));
    mags->push_back(new MagPoint(this));
    mags->push_back(new MagPoint(this));
    setRect(QRectF(-w/2, -h/2, w, h));
}

void DDiaItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    setBrush(QBrush(Qt::transparent));
    painter->setBrush(brush());
    painter->setPen(pen());


    // 创建菱形的四个顶点
    QPolygonF polygon;
    polygon << QPointF(rect.center().x(), rect.top())   // 顶部中心点
            << QPointF(rect.right(), rect.center().y())  // 右侧中心点
            << QPointF(rect.center().x(), rect.bottom())  // 底部中心点
            << QPointF(rect.left(), rect.center().y());   // 左侧中心点

    painter->drawPolygon(polygon);
}

QRectF DDiaItem::sizeRect() const
{
    return rect;
}

QPainterPath DDiaItem::shapeNormal() const
{
    QPainterPath path;
    // 创建菱形的四个顶点
    QPolygonF polygon;
    polygon << QPointF(rect.center().x(), rect.top())   // 顶部中心点
            << QPointF(rect.right(), rect.center().y())  // 右侧中心点
            << QPointF(rect.center().x(), rect.bottom())  // 底部中心点
            << QPointF(rect.left(), rect.center().y());   // 左侧中心点

    path.addPolygon(polygon);
    return path;
}

void DDiaItem::updateMagPoint()
{
    // 菱形的四个角点
    (*mags)[0]->pos = {rect.center().x(), rect.top()};
    (*mags)[1]->pos = {rect.right(), rect.center().y()};
    (*mags)[2]->pos = {rect.center().x(), rect.bottom()};
    (*mags)[3]->pos = {rect.left(), rect.center().y()};
}

void DDiaItem::sizeToRect(QRectF nrect)
{
    setRect(nrect);
}

void DDiaItem::modiToPoint(QPointF p, int id)
{
    Q_UNUSED(p); Q_UNUSED(id);
    // 这里可以实现修改控制点时的具体行为
}

void DDiaItem::setRect(const QRectF &nrect)
{
    rect = nrect;
    sizeRectUpdated();
    updateMagPoint();
}
