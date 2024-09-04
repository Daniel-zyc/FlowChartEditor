#include "dmultidocitem.h"
#include "magpoint.h"

DMultiDocItem::DMultiDocItem(QGraphicsItem *parent)
    : DMultiDocItem(minRectSize, minRectSize, parent) {}

DMultiDocItem::DMultiDocItem(qreal w, qreal h, QGraphicsItem *parent)
    : DShapeBase("", parent)
{
    for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
    rect = QRectF(-w/2, -h/2, w, h);
    updateSizePoint();
    updatePath();
    updateMagPoint();
}

void DMultiDocItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    painter->setBrush(brush());
    painter->setPen(pen());
    painter->drawPath(path);
}

QRectF DMultiDocItem::sizeRect() const
{
    return rect;
}

QPainterPath DMultiDocItem::shapeNormal() const
{
    return path;
}

void DMultiDocItem::sizeToRect(QRectF nrect)
{
    rect = nrect;
    updateSizePoint();
    updatePath();
    updateMagPoint();
}

void DMultiDocItem::modiToPoint(QPointF p, int id)
{
    Q_UNUSED(p); Q_UNUSED(id);
    return;
}

void DMultiDocItem::updatePath()
{
    path.clear();

    path.moveTo(rect.topLeft());
    path.lineTo(rect.topRight());
    path.lineTo(rect.bottomRight() - QPointF(0, rect.height() / 5));

    // 设置贝塞尔曲线的四个控制点
    QPointF startPoint(50, 250);     // 起点
    QPointF controlPoint1(150, 50);  // 第一个控制点
    QPointF controlPoint2(250, 450); // 第二个控制点
    QPointF endPoint(350, 250);      // 终点

    // 从起点开始路径
    path.moveTo(startPoint);

    // 绘制三次贝塞尔曲线
    path.cubicTo(controlPoint1, controlPoint2, endPoint);


}

void DMultiDocItem::updateMagPoint()
{
    (*mags)[0]->setPos({rect.left(), -rect.height() / 10});
    (*mags)[1]->setPos({rect.right(), -rect.height() / 10});

    (*mags)[2]->setPos({0, rect.top()});
    (*mags)[3]->setPos({0, rect.bottom() * 4 / 5});
}

