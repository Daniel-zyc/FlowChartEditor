#include "ddocitem.h"
#include "magpoint.h"

DDocItem::DDocItem(QGraphicsItem *parent)
    : DShapeBase(parent) {}

DDocItem::DDocItem(qreal w, qreal h, QGraphicsItem *parent)
    : DShapeBase(parent)
{
    mags->push_back(new MagPoint(this));
    mags->push_back(new MagPoint(this));
    mags->push_back(new MagPoint(this));
    mags->push_back(new MagPoint(this));
    setRect(QRectF(-w/2, -h/2, w, h));
}

QPainterPath DDocItem::addDocShape(const QRectF &rec) const {
    QPainterPath path;

    path.moveTo(rec.topLeft());
    path.lineTo(rec.topRight());
    path.lineTo(rec.bottomRight() - QPointF(0, rec.height() / 5));

    qreal waveLength = rec.width() / 2;

    QPointF startPoint = rec.bottomRight() - QPointF(0, rec.height() / 5);

    for (int i = 0; i < 2; ++i) {
        QPointF controlPoint;
        QPointF endPoint;
        if(i == 0){
            controlPoint = QPointF(startPoint.x() - waveLength / 2, startPoint.y() + 2 * (i % 2 == 1 ? 1 : -1));
            endPoint = QPointF(startPoint.x() - waveLength, startPoint.y() + rec.height() / 10);
        }
        else{
            controlPoint = QPointF(startPoint.x() - waveLength / 2, startPoint.y() +rec.height() / 6 * (i % 2 == 1 ? 1 : -1));
            endPoint = QPointF(startPoint.x() - waveLength, startPoint.y());
        }
        path.quadTo(controlPoint, endPoint);
        startPoint = endPoint;
    }
    path.lineTo(rec.bottomLeft() - QPointF(0, rec.height() / 10));
    path.closeSubpath();

    return path;
}

void DDocItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    // setBrush(QBrush(Qt::transparent));
    painter->setBrush(brush());
    painter->setPen(pen());
    QPainterPath docShape = addDocShape(rect);
    painter->drawPath(docShape);
}

QRectF DDocItem::sizeRect() const
{
    return rect;
}

QPainterPath DDocItem::shapeNormal() const
{
    QPainterPath pth;
    pth.addRect(rect);
    return pth;
}

void DDocItem::updateMagPoint()
{
    (*mags)[0]->pos = {rect.left(), 0};
    (*mags)[1]->pos = {rect.right(), 0};

    (*mags)[2]->pos = {0, rect.top()};
    (*mags)[3]->pos = {0, rect.bottom() * 4 / 5};
}

void DDocItem::sizeToRect(QRectF nrect)
{
    setRect(nrect);
}

void DDocItem::modiToPoint(QPointF p, int id)
{
    Q_UNUSED(p); Q_UNUSED(id);
    return;
}

void DDocItem::setRect(const QRectF &nrect)
{
    rect = nrect;
    sizeRectUpdated();
    updateMagPoint();
}
