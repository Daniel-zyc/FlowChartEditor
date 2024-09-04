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

    qreal waveLength = rect.width() / 2;

    QPointF startPoint = rect.bottomRight() - QPointF(0, rect.height() / 5);
    QPointF controlPoint, endPoint;

    controlPoint = QPointF(startPoint.x() - waveLength / 2, startPoint.y() - 2);
    endPoint = QPointF(startPoint.x() - waveLength, startPoint.y() + rect.height() / 10);
    path.quadTo(controlPoint, endPoint);
    startPoint = endPoint;

    controlPoint = QPointF(startPoint.x() - waveLength / 2, startPoint.y() + rect.height() / 6 * 1);
    endPoint = QPointF(startPoint.x() - waveLength, startPoint.y());
    path.quadTo(controlPoint, endPoint);
    startPoint = endPoint;

    path.lineTo(rect.bottomLeft() - QPointF(0, rect.height() / 10));
    path.closeSubpath();
}

void DMultiDocItem::updateMagPoint()
{
    (*mags)[0]->setPos({rect.left(), -rect.height() / 10});
    (*mags)[1]->setPos({rect.right(), -rect.height() / 10});

    (*mags)[2]->setPos({0, rect.top()});
    (*mags)[3]->setPos({0, rect.bottom() * 4 / 5});
}

