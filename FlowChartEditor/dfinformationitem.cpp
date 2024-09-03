#include "dfinformationitem.h"
#include "magpoint.h"

DFInformationItem::DFInformationItem(QGraphicsItem *parent)
    : DShapeBase(parent) {}

DFInformationItem::DFInformationItem(qreal w, qreal h, QGraphicsItem *parent)
    : DShapeBase(parent)
{
    mags->push_back(new MagPoint(this));
    mags->push_back(new MagPoint(this));
    mags->push_back(new MagPoint(this));
    mags->push_back(new MagPoint(this));
    setRect(QRectF(-w/2, -h/2, w, h));
}

QPainterPath DFInformationItem::addInfShape(const QRectF &rec) const
{
    QPainterPath path;

    path.moveTo(rec.topLeft() + QPointF(0, rec.height() / 10));

    qreal waveLength = rec.width() / 2;

    QPointF controlPoint1 = QPointF(rec.topLeft().x() + waveLength / 2, rec.topLeft().y() + rec.height() / 4);
    QPointF controlPoint2 = QPointF(rec.topRight().x() - waveLength / 2, rec.topLeft().y() - rec.height() / 10);
    QPointF controlPoint3 = QPointF(rec.bottomRight().x() - waveLength / 2, rec.bottomLeft().y() - rec.height() / 4);
    QPointF controlPoint4 = QPointF(rec.bottomLeft().x() + waveLength / 2, rec.bottomLeft().y() + rec.height() / 10);

    QPointF endPoint1 = (rec.topLeft() + rec.topRight()) / 2 + QPointF(0, rec.height() / 10);
    QPointF endPoint2 = rec.topRight() + QPointF(0, rec.height() / 10);
    QPointF endPoint3 = (rec.bottomLeft() + rec.bottomRight()) / 2 - QPointF(0, rec.height() / 10);
    QPointF endPoint4 = rec.bottomLeft() - QPointF(0, rec.height() / 10);

    path.quadTo(controlPoint1, endPoint1);
    path.quadTo(controlPoint2, endPoint2);

    path.lineTo(rec.bottomRight() - QPointF(0, rec.height() / 10));

    path.quadTo(controlPoint3, endPoint3);
    path.quadTo(controlPoint4, endPoint4);

    path.closeSubpath();

    return path;
}

void DFInformationItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    //setBrush(QBrush(QT::transparent));
    painter->setBrush(brush());
    painter->setPen(pen());
    QPainterPath infShape = addInfShape(rect);
    painter->drawPath(infShape);
}

QRectF DFInformationItem::sizeRect() const
{
    return rect;
}

QPainterPath DFInformationItem::shapeNormal() const
{
    QPainterPath pth;
    pth.addRect(rect);
    return pth;
}

void DFInformationItem::updateMagPoint()
{
    (*mags)[0]->pos = {rect.left(), 0};
    (*mags)[1]->pos = {rect.right(), 0};

    (*mags)[2]->pos = {0, rect.top()};
    (*mags)[3]->pos = {0, rect.bottom() * 9 / 10};
}

void DFInformationItem::sizeToRect(QRectF nrect)
{
    setRect(nrect);
}

void DFInformationItem::modiToPoint(QPointF p, int id)
{
    Q_UNUSED(p); Q_UNUSED(id);
    return;
}

void DFInformationItem::setRect(const QRectF &nrect)
{
    rect = nrect;
    updateSizePoint();
    updateMagPoint();
}

//====================================

// void DInfItem::serialize(QDataStream &out) const{
//     qDebug() << "DDocItem serializing";
//     DShapeBase::serialize(out);

//     out << rect;
// }

// void DInfItem::deserialize(QDataStream &in){
//     qDebug() << "DDocItem deserializing";
//     DShapeBase::deserialize(in);

//     in >> rect;
//     setRect(rect);
// }
