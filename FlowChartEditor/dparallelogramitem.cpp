#include "dparallelogramitem.h"
#include "magpoint.h"

DParallelogramItem::DParallelogramItem(QGraphicsItem *parent)
    : DShapeBase(parent) {}

DParallelogramItem::DParallelogramItem(qreal w, qreal h, QGraphicsItem *parent)
    : DShapeBase(parent)
{
    skew = w /4 ;
    modis.resize(2);
    mags->push_back(new MagPoint(this));
    mags->push_back(new MagPoint(this));
    mags->push_back(new MagPoint(this));
    mags->push_back(new MagPoint(this));
    mags->push_back(new MagPoint(this));
    mags->push_back(new MagPoint(this));
    setRect(QRectF(-w/2, -h/2, w, h));
}

QPainterPath DParallelogramItem::addParallelogram(const QRectF &rect, qreal skew) const
{
    QPainterPath path;

    QPointF p1(rect.topLeft() + QPointF(skew, 0));
    QPointF p2(rect.topRight());
    QPointF p3(rect.bottomRight() - QPointF(skew, 0));
    QPointF p4(rect.bottomLeft());

    path.moveTo(p1);
    path.lineTo(p2);
    path.lineTo(p3);
    path.lineTo(p4);
    path.closeSubpath();

    return path;
}

void DParallelogramItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    // setBrush(QBrush(Qt::transparent));
    painter->setBrush(brush());
    painter->setPen(pen());
    QPainterPath parallelogramPath = addParallelogram(rect, skew);
    painter->drawPath(parallelogramPath);
}

QRectF DParallelogramItem::sizeRect() const
{
    return rect;
}

QPainterPath DParallelogramItem::shapeNormal() const
{
    QPainterPath pth;
    pth = addParallelogram(rect, skew);
    return pth;
}

void DParallelogramItem::updateMagPoint()
{
    double pos = rect.height() / 2 * (1.0 * rect.width() / skew - 1);
    (*mags)[0]->pos = {rect.left() + skew / 2, 0};
    (*mags)[1]->pos = {rect.right() - skew / 2, 0};
    (*mags)[2]->pos = {skew/2, rect.top()};
    (*mags)[3]->pos = {-1 * skew/2, rect.bottom()};
    if(skew <= rect.width() / 2){
        (*mags)[4]->pos = {0, rect.top()};
        (*mags)[5]->pos = {0, rect.bottom()};
    }
    else{
        (*mags)[4]->pos = {0, -pos};
        (*mags)[5]->pos = {0, pos};
    }
}

void DParallelogramItem::updateModiPoint()
{
    skew = qMin(skew, rect.width());
    modis[0] = {rect.left() + skew, rect.top()};
    modis[1] = {rect.right() - skew, rect.bottom()};
}

void DParallelogramItem::sizeToRect(QRectF nrect)
{
    setRect(nrect);
}

void DParallelogramItem::modiToPoint(QPointF p, int id)
{
    switch(id)
    {
        case 0:
            skew = qAbs(rect.left() - p.x());
            updateModiPoint();
            updateMagPoint();
            break;
        case 1:
            skew = qAbs(rect.right() - p.x());
            updateModiPoint();
            updateMagPoint();
            break;
    }
    return;
}

void DParallelogramItem::setRect(const QRectF &nrect)
{
    rect = nrect;
    updateSizePoint();
    updateMagPoint();
    updateModiPoint();
}

//====================================

// void DParallelogramItem::serialize(QDataStream &out) const{
//     qDebug() << "DParallegramItem serializing";
//     DShapeBase::serialize(out);

//     out << rect << skew;
// }

// void DParallelogramItem::deserialize(QDataStream &in){
//     qDebug() << "DParallegramItem deserializing";
//     DShapeBase::deserialize(in);

//     in >> rect >> skew;
//     setRect(rect);
// }
