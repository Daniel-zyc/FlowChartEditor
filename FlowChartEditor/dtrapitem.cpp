#include "dtrapitem.h"
#include "magpoint.h"

DTrapItem::DTrapItem(QGraphicsItem *parent)
    : DShapeBase(parent){}

DTrapItem::DTrapItem(qreal topWidth,qreal bottomWidth, qreal height, QGraphicsItem *parent)
    : DTrapItem(parent)
{
    this->topWidth = topWidth;
    this->bottomWidth = bottomWidth;
    this->height = height;

    modis.resize(1); // 只有一个调整点
    mags->push_back(new MagPoint(this));
    mags->push_back(new MagPoint(this));
    mags->push_back(new MagPoint(this));
    mags->push_back(new MagPoint(this));
    setRect(QRectF(-topWidth/2, -height/2, topWidth, height));
    c=bottomWidth/rect.width();

}
void DTrapItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    setBrush(QBrush(Qt::transparent));
    painter->setBrush(brush());
    painter->setPen(pen());

    QPolygonF polygon;
    polygon<< QPointF(rect.left(), rect.top())
            << QPointF(rect.right(), rect.top())
            << QPointF(rect.right() - (rect.width()- c*rect.width()) / 2, rect.bottom())
            << QPointF(rect.left() + (rect.width()- c*rect.width()) / 2, rect.bottom());

    painter->drawPolygon(polygon);
    updateMagPoint();
    updateModiPoint();
}

QRectF DTrapItem::sizeRect() const
{
    return rect;
}

QPainterPath DTrapItem::shapeNormal() const
{
    QPainterPath path;
    QPolygonF polygon;
    polygon<< QPointF(rect.left(), rect.top())
            << QPointF(rect.right(), rect.top())
            << QPointF(rect.right() - (rect.width()- c*rect.width()) / 2, rect.bottom())
            << QPointF(rect.left() + (rect.width()- c*rect.width()) / 2, rect.bottom());
    path.addPolygon(polygon);
    return path;
}

void DTrapItem::updateMagPoint()
{
    // 更新磁吸点为各边的中点
    (*mags)[0]->pos = {(rect.left() + rect.right()) / 2, rect.top()};
    (*mags)[1]->pos = {(rect.left() + rect.right()) / 2 +c*rect.width()/2+(rect.width()-c*rect.width())/4 , (rect.top() + rect.bottom()) / 2};
    (*mags)[2]->pos = {(rect.left() + rect.right()) / 2, rect.bottom()};
    (*mags)[3]->pos = {(rect.left() + rect.right()) / 2 -c*rect.width()/2-(rect.width()-c*rect.width())/4, (rect.top() + rect.bottom()) / 2};
}

void DTrapItem::updateModiPoint()
{
    // 只有一个调整点
    modis[0] = {rect.left() + (rect.width() - c*rect.width()) / 2, rect.bottom()};
}

void DTrapItem::sizeToRect(QRectF nrect)
{
    setRect(nrect);
}

void DTrapItem::modiToPoint(QPointF p, int id)
{
    if (id == 0) {
        if(((rect.right()-p.x())>=rect.width()/2)){
            qreal newBottomWidth = ((rect.right() - p.x())-rect.width()/2)*2;
            bottomWidth = qMin(newBottomWidth, rect.width());
        }else{
            bottomWidth =0;
        }

        topWidth = rect.width();
        c=bottomWidth/rect.width();
        updateModiPoint();
    }
}

void DTrapItem::setRect(const QRectF &nrect)
{
    rect = nrect;
    sizeRectUpdated();
    updateMagPoint();
    updateModiPoint();
}

//================================
// void DTrapItem::serialize(QDataStream &out) const{
//     DShapeBase::serialize(out);
//     out << topWidth << bottomWidth;
//     out << height << c << rect;
// }

// void DTrapItem::deserialize(QDataStream &in){
//     DShapeBase::deserialize(in);
//     in << topWidth << bottomWidth;
//     in << height << c << rect;
// }
