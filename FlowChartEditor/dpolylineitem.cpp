#include "dpolylineitem.h"

DPolyLineItem::DPolyLineItem(QGraphicsItem *parent)
    :DLineBase(parent) {}

DPolyLineItem::DPolyLineItem(QPointF begin, QPointF end, QGraphicsItem *parent)
    : DLineBase(parent)
{
    beginPoint = begin;
    endPoint = end;
    //重置调整点数量
    modis.resize(modis_num);
    updatePosition();
}

QRectF DPolyLineItem::boundingRect() const
{
    qreal r = maxPointRadius;
    int direct = getPolyLineDirection();
    //所有影响框线的点
    QVector<QPointF> points = {
        QPointF(beginPoint.x(), beginPoint.y()),
        QPointF(beginPoint.x() + st_x_offset, beginPoint.y() + st_y_offset),
        QPointF((beginPoint.x() + st_x_offset) * direct + mid_x_shift * (direct ^ 1),
                (beginPoint.y() + st_y_offset) * (direct ^ 1) + mid_y_shift * direct),
        QPointF((endPoint.x() + ed_x_offset) * direct + mid_x_shift * (direct ^ 1),
                (endPoint.y() + ed_y_offset) * (direct ^ 1) + mid_y_shift * direct),
        QPointF(endPoint.x() + ed_x_offset, endPoint.y() + ed_y_offset),
        QPointF(endPoint.x(), endPoint.y())
    };
    QPointF minPoint = getMinPoint(points),maxPoint = getMaxPoint(points);
    QSizeF sz(maxPoint.x() - minPoint.x(), maxPoint.y() - minPoint.y());
    return QRectF(minPoint, sz).normalized().adjusted(-r, -r, r, r);
}

QPointF DPolyLineItem::getMinPoint(QVector<QPointF> points) const
{
    double minx = 0x3f3f3f3f, miny = 0x3f3f3f3f;
    for(int i = 0; i < points.size(); i++) {
        if(points[i].x() < minx) minx = points[i].x();
        if(points[i].y() < miny) miny = points[i].y();
    }
    return QPointF(minx,miny);
}
QPointF DPolyLineItem::getMaxPoint(QVector<QPointF> points) const
{
    double maxx = 0, maxy = 0;
    for(int i = 0; i < points.size(); i++) {
        if(points[i].x() > maxx) maxx = points[i].x();
        if(points[i].y() > maxy) maxy = points[i].y();
    }
    return QPointF(maxx,maxy);
}

void DPolyLineItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    QBrush qbrush = painter->brush();
    qbrush.setColor(pen().color());
    setBrush(qbrush);

    painter->setBrush(brush());
    painter->setPen(pen());

    int direct = getPolyLineDirection();
    QPointF points[6] = {
        QPointF(beginPoint.x(), beginPoint.y()),
        QPointF(beginPoint.x() + st_x_offset, beginPoint.y() + st_y_offset),
        QPointF((beginPoint.x() + st_x_offset) * direct + mid_x_shift * (direct ^ 1),
                (beginPoint.y() + st_y_offset) * (direct ^ 1) + mid_y_shift * direct),
        QPointF((endPoint.x() + ed_x_offset) * direct + mid_x_shift * (direct ^ 1),
                (endPoint.y() + ed_y_offset) * (direct ^ 1) + mid_y_shift * direct),
        QPointF(endPoint.x() + ed_x_offset, endPoint.y() + ed_y_offset),
        QPointF(endPoint.x(), endPoint.y())
    };
    for(int i = 0; i < 6; i++ )
        qDebug() << points[i].x() << ":" << points[i].y();
    painter->drawPolyline(points,6);
}

void DPolyLineItem::modiToPoint(QPointF p, int id)
{
    switch(id)
    {
    case 0:
        if(getPolyLineDirection()) mid_y_shift = p.y();
        else mid_x_shift = p.x();
        updateModiPoint();
        break;
    case 1:

        updateModiPoint();
        break;
    case 2:

        updateModiPoint();
        break;
    }
    return;

}


QPainterPath DPolyLineItem::shapeNormal() const
{
    QGraphicsLineItem item(QLineF(beginPoint, endPoint));
    return item.shape();
}

void DPolyLineItem::updateLine()
{
    updatePolyLineType();
    updateMidPoint(line_type);
    updateModiPoint();
    return;
}
//中心定位点
void DPolyLineItem::updateMidPoint(int type)
{
    switch (type) {
    case 0:
        mid_x_shift = (beginPoint.x() + st_x_offset + endPoint.x() + ed_x_offset) / 2;
        mid_y_shift = (beginPoint.y() + st_y_offset + endPoint.y() + ed_y_offset) / 2;
        break;
    case 1:
        mid_x_shift = beginPoint.x();
        mid_y_shift = endPoint.y();
        break;
    case 2:
        mid_x_shift = midPoint.x();
        mid_y_shift = midPoint.y();
        break;
    }
    return;
}

void DPolyLineItem::updateModiPoint()
{
    if(line_type) {
        modis_num = 0;
        modis.resize(0);
    } else {
        modis_num = 1;
        modis.resize(1);
        modis[0] = QPointF(mid_x_shift,mid_y_shift);
        int direct = getPolyLineDirection();
        if((abs(st_x_offset) > 1e-6 || abs(st_y_offset) > 1e-6) && direct) {
            modis_num++;
            modis.resize(modis_num);
            modis[modis_num - 1] = QPointF((beginPoint.x() + st_x_offset * direct + mid_x_shift * (direct ^ 1) + beginPoint.x() + st_x_offset) / 2,
                                           (beginPoint.y() + st_y_offset + (beginPoint.y() + st_y_offset) * (direct ^ 1) + mid_y_shift * direct) / 2);
        }
        if((abs(ed_x_offset) > 1e-6 || abs(ed_y_offset) > 1e-6) && !direct) {
            modis_num++;
            modis.resize(modis_num);
            modis[modis_num - 1] = QPointF((endPoint.x() + ed_x_offset * direct + mid_x_shift * (direct ^ 1) + endPoint.x() + ed_x_offset) / 2,
                                           (endPoint.y() + ed_y_offset + (endPoint.y() + ed_y_offset) * (direct ^ 1) + mid_y_shift * direct) / 2);
        }

    }
    return;
}
//折线中间线的方向横1/竖0
int DPolyLineItem::getPolyLineDirection() const
{
    if(abs(endPoint.x() - beginPoint.x()) > abs(endPoint.y() - beginPoint.y())) return 0;
    else return 1;
}

//上右下左1234
int DPolyLineItem::getCollideDirection(QRectF item,QPointF center)
{
    int item_direct = 0;
    if(abs(center.x() + item.left() - beginPoint.x()) < 50) item_direct = 4;
    else if(abs(center.x() + item.right() - beginPoint.x()) < 50) item_direct = 2;
    if(abs(center.y() + item.top() - beginPoint.y()) < 50) item_direct = 1;
    else if(abs(center.y() + item.bottom() - beginPoint.y()) < 50) item_direct = 3;
    return item_direct;
}
//相对位置 1234右上开始顺时针
int DPolyLineItem::getRelativePosition(QPointF cur,QPointF another)
{
    if(another.x() > cur.x())
        return (another.y() > cur.y()) ? 2 : 1;
    else return (another.y() > cur.y()) ? 3 : 4;
}

void DPolyLineItem::updatePolyLineType()
{
    //测试方向 1234上右下左
    int begin_item_direct = 0, end_item_direct = 0;
    QRectF st_item, ed_item;
    QPointF st_center,ed_center;
    if(beginMag) {
        st_item = beginMag->parent->boundingRect();
        st_center = beginMag->parent->pos();
        begin_item_direct = getCollideDirection(st_item,st_center);
    }
    if(endMag) {
        ed_item = endMag->parent->boundingRect();
        ed_center = endMag->parent->pos();
        end_item_direct = getCollideDirection(ed_item,ed_center);
    }
    qDebug() << "directions:" << begin_item_direct << "," << end_item_direct;
    updateOffsets(begin_item_direct,end_item_direct);
    return;
}

QPointF DPolyLineItem::getBoundingPoint(int point, int type)
{
    QRectF item,bound;
    if(!point && beginMag) {
        item = beginMag->parent->boundingRect();
        bound = beginMag->parent->mapRectToScene(item);
    } else if(point && endMag) {
        item = endMag->parent->boundingRect();
        bound = endMag->parent->mapRectToScene(item);
    }
    switch (type) {
    case 1:
        return QPointF(bound.right() + 40 , bound.top() - 40);
        break;
    case 2:
        return QPointF(bound.right() + 40 , bound.bottom() + 40);
        break;
    case 3:
        return QPointF(bound.left() - 40 , bound.bottom() + 40);
        break;
    case 4:
        return QPointF(bound.left() - 40 , bound.top() - 40);
        break;
    }
    return QPointF(0,0);
}

void DPolyLineItem::updateOffsets(int st_dir,int ed_dir)
{
    int relative_pos = getRelativePosition(beginPoint,endPoint);
    int rect_type = getPolyLineDirection();
    int temp_relative_pos;
    switch (st_dir) {
    case 0:
        st_x_offset = 0;
        st_y_offset = 0;
        line_type = 0;
        return;
    case 1:
        st_y_offset = -40;
        st_x_offset = 0;
        temp_relative_pos = getRelativePosition(QPointF(beginPoint.x() + st_x_offset, beginPoint.y() + st_y_offset),endPoint);
        if(!rect_type) {
            if(temp_relative_pos == 1 || temp_relative_pos == 4) line_type = 1;
            else line_type = 0;
        } else {
            if(temp_relative_pos == 1 || temp_relative_pos == 4) st_y_offset = st_x_offset = 0;
            else if(temp_relative_pos == 2) {
                line_type = 3;
                //qDebug() << "midPoint:" << midPoint;
                //midPoint = getBoundingPoint(0,1);

            } else if(temp_relative_pos == 3) {
                line_type = 3;
                //midPoint = getBoundingPoint(0,2);
                //qDebug() << "midPoint:" << midPoint;
            }
            else line_type = 0;
        }
        break;
    case 2:
        st_x_offset = 40;
        st_y_offset = 0;
        if(rect_type) {
            temp_relative_pos = getRelativePosition(QPointF(beginPoint.x() + st_x_offset, beginPoint.y() + st_y_offset),endPoint);
            if(temp_relative_pos == 1 || temp_relative_pos == 2) line_type = 2;
            else line_type = 0;
        } else line_type = 0;
        break;
    case 3:
        st_y_offset = 40;
        st_x_offset = 0;
        if(!rect_type) {
            temp_relative_pos = getRelativePosition(QPointF(beginPoint.x() + st_x_offset, beginPoint.y() + st_y_offset),endPoint);
            if(temp_relative_pos == 2 || temp_relative_pos == 3) line_type = 1;
            else line_type = 0;
        } else line_type = 0;
        break;
    case 4:
        st_x_offset = -40;
        st_y_offset = 0;
        if(rect_type) {
            temp_relative_pos = getRelativePosition(QPointF(beginPoint.x() + st_x_offset, beginPoint.y() + st_y_offset),endPoint);
            if(temp_relative_pos == 3 || temp_relative_pos == 4) line_type = 2;
            else line_type = 0;
        } else line_type = 0;
        break;
    }
    /*
    switch (ed_dir) {
    case 0:
        ed_x_offset = 0;
        ed_y_offset = 0;
        line_type = 0;
        return;
    case 1:
        ed_x_offset = -40;
        ed_y_offset = 0;
        line_type = 3;
        break;
    case 2:
        ed_x_offset = 0;
        ed_y_offset = 40;
        line_type = 4;
        break;
    case 3:
        ed_x_offset = 40;
        ed_y_offset = 0;
        line_type = 1;
        break;
    case 4:
        ed_x_offset = 0;
        ed_y_offset = -40;
        line_type = 2;
        break;
    }
*/
}
