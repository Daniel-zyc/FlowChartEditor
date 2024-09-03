#include "dcurvelineitem.h"
#include "magpoint.h"

DCurveLineItem::DCurveLineItem(QGraphicsItem *parent)
    : DLineBase(parent) {
    qDebug() << tip;
}

DCurveLineItem::DCurveLineItem(QPointF begin, QPointF end, QGraphicsItem *parent)
    : DLineBase(parent)
{
    modis.resize(1);
    beginPoint = begin;
    endPoint = end;
    tip = (beginPoint + endPoint) / 2;
    updatePosition();
}

QRectF DCurveLineItem::boundingRect() const
{
    qreal r = maxPointRadius;
    qreal w,h;
    QPointF startPoint = beginPoint;;

    if((tip.x() >= beginPoint.x() && tip.x() <= endPoint.x()) || (tip.x() <= beginPoint.x() && tip.x() >= endPoint.x())){
        w = endPoint.x() - beginPoint.x();
    }
    else{
        if((beginPoint.x() < endPoint.x() && tip.x() < beginPoint.x()) || (beginPoint.x() > endPoint.x() && tip.x() < endPoint.x())){
            w = qMax(beginPoint.x(), endPoint.x()) - tip.x();
            startPoint = QPointF(tip.x(), beginPoint.y());
        }
        else{
            w = tip.x() - qMin(beginPoint.x(), endPoint.x());
            startPoint = QPointF(qMin(beginPoint.x(), endPoint.x()), beginPoint.y());
        }
    }

    if((tip.y() >= beginPoint.y() && tip.y() <= endPoint.y()) || (tip.y() <= beginPoint.y() && tip.y() >= endPoint.y())){
        h = endPoint.y() - beginPoint.y();
    }
    else{
        if((beginPoint.y() < endPoint.y() && tip.y() < beginPoint.y()) || (beginPoint.y() > endPoint.y() && tip.y() < endPoint.y())){
            h = tip.y() - qMax(beginPoint.y(), endPoint.y());
            startPoint = QPointF(beginPoint.x(), qMax(beginPoint.y(),endPoint.y()));
        }
        else{
            h = qMin(beginPoint.y(), endPoint.y()) - tip.y();
            startPoint = QPointF(beginPoint.x(), tip.y());
        }
    }

    QSizeF sz(w, h);
    return QRectF(startPoint, sz).normalized().adjusted(-r, -r, r, r);
}

QPainterPath DCurveLineItem::drawCurve() const
{

    QPainterPath path;

    path.moveTo(beginPoint);

    QPointF controlPoint;
    qreal waveHeight;

    int bulge = 1;
    if(endPoint.y() > beginPoint.y()){
        bulge = -bulge;
    }
    if(qAbs(angle) > DConst::PI / 4 && qAbs(angle) < DConst::PI * 3 / 4){
        bulge = -bulge;
        if(endPoint.x() < beginPoint.x()){
            bulge = -bulge;
        }
        if(endPoint.y() > beginPoint.y()){
            bulge = -bulge;
        }
    }
    if(qAbs(angle) <= DConst::PI / 4 || qAbs(angle) >= DConst::PI * 3 / 4){
        waveHeight = qAbs(beginPoint.y() - endPoint.y()) / 2;

        controlPoint = QPointF(beginPoint.x() * 1 / 10 + tip.x() * 9 / 10, beginPoint.y() * 1 / 10 + tip.y() * 9 / 10 + waveHeight * bulge);
        path.quadTo(controlPoint, tip);

        controlPoint = QPointF(tip.x() * 9 / 10 + endPoint.x() * 1 / 10, tip.y() * 9 / 10 + endPoint.y() * 1 / 10 - waveHeight * bulge);
        path.quadTo(controlPoint, endPoint);
    }
    else{
        waveHeight = qAbs(beginPoint.x() - endPoint.x()) / 2;

        controlPoint = QPointF(beginPoint.x() * 1 / 10 + tip.x() * 9 / 10 + waveHeight * bulge, beginPoint.y() * 1 / 10 + tip.y() * 9 / 10);
        path.quadTo(controlPoint, tip);

        controlPoint = QPointF(tip.x() * 9 / 10 + endPoint.x() * 1 / 10 - waveHeight * bulge, tip.y() * 9 / 10 + endPoint.y() * 1 / 10);
        path.quadTo(controlPoint, endPoint);
    }

    return path;
}

void DCurveLineItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    QBrush qbrush = painter->brush();
    qbrush.setColor(pen().color());
    setBrush(qbrush);

    painter->setBrush(brush());
    painter->setPen(pen());

    painter->drawPath(drawCurve());

    double arrowAngle;

    if(qAbs(angle) < DConst::PI / 4){
        arrowAngle = 0;
    }
    else{
        if(qAbs(angle) < DConst::PI * 3 / 4){
            arrowAngle = DConst::PI / 2 * (angle > 0 ? 1 : -1);
        }
        else{
            arrowAngle = DConst::PI;
        }
    }

    if((endPoint.x() > beginPoint.x() && tip.x() > endPoint.x()) || (endPoint.x() < beginPoint.x() && tip.x() < endPoint.x())){
        arrowAngle = (arrowAngle == 0 ? DConst::PI : 0);
    }
    if((endPoint.y() > beginPoint.y() && tip.y() > endPoint.y()) || (endPoint.y() < beginPoint.y() && tip.y() < endPoint.y())){
        arrowAngle *= -1;
    }

    drawArrow(painter, arrowAngle, endPoint, endArrowType);
}

void DCurveLineItem::updateModiPoint()
{
    modis[0] = tip;
}

void DCurveLineItem::modiToPoint(QPointF p, int id)
{
    Q_UNUSED(id);
    if(qAbs(angle) < DConst::PI / 4 || qAbs(angle) > DConst::PI * 3 / 4){
        tip.setX(p.x());
    }
    else{
        tip.setY(p.y());
    }
    updateModiPoint();
    return;
}

QPainterPath DCurveLineItem::shapeNormal() const
{
    return drawCurve();
}

void DCurveLineItem::updateLine()
{
    tip = (beginPoint + endPoint) / 2;
    angle = getAngle(beginPoint, endPoint);
    updateModiPoint();
}

//================================

// void DCurveItem::serialize(QDataStream &out) const{
//     // qDebug() << "DLineItem serializing";
//     DLineBase::serialize(out);

//     out << tip << angle;
// }

// void DCurveItem::deserialize(QDataStream &in){
//     // qDebug() << "DLineItem deserializing";
//     DLineBase::deserialize(in);

//     in << tip << angle;
// }
