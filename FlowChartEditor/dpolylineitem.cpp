#include "dpolylineitem.h"

DPolyLineItem::DPolyLineItem(QGraphicsItem *parent)
    :DLineBase(parent) {}

DPolyLineItem::DPolyLineItem(QPointF begin, QPointF end, QGraphicsItem *parent)
    : DLineBase(parent)
{
    beginPoint = begin;
    endPoint = end;
    //重置调整点数量
    updatePolyLineType(); // 实时更新？
    updateBeginMidPoint(line_type);
    updateEndMidPoint(line_type);
    modis.resize(modis_num);
    updatePosition();
}

QRectF DPolyLineItem::boundingRect() const
{
    qreal r = maxPointRadius;
    int direct = getPaintDirection();
    //所有影响框线的点
    QVector<QPointF> points = {
        QPointF(beginPoint.x(), beginPoint.y()),
        QPointF(beginPoint.x() + st_x_offset, beginPoint.y() + st_y_offset),
        begin_midPoint,
        QPointF(begin_midPoint.x() * direct + end_midPoint.x() * (direct ^ 1),
                begin_midPoint.y() * (direct ^ 1) + end_midPoint.x() * direct),
        QPointF(begin_midPoint.x() * (direct ^ 1) + end_midPoint.x() * direct,
                begin_midPoint.y() * direct + end_midPoint.x() * (direct ^ 1)),
        end_midPoint,
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

    int direct = getPaintDirection();
    QPointF points[6] = {
        QPointF(beginPoint.x(), beginPoint.y()),
        QPointF(beginPoint.x() + st_x_offset, beginPoint.y() + st_y_offset),
        //begin_midPoint,
        QPointF(begin_midPoint.x() * direct + end_midPoint.x() * (direct ^ 1),
                begin_midPoint.y() * (direct ^ 1) + end_midPoint.y() * direct),
        QPointF(begin_midPoint.x() * (direct ^ 1) + end_midPoint.x() * direct,
                begin_midPoint.y() * direct + end_midPoint.y() * (direct ^ 1)),
        //end_midPoint,
        QPointF(endPoint.x() + ed_x_offset, endPoint.y() + ed_y_offset),
        QPointF(endPoint.x(), endPoint.y())
    };
    painter->drawPolyline(points,6);
    painter->setBrush(Qt::red);
    painter->drawEllipse(begin_midPoint,5,5);
    painter->drawEllipse(end_midPoint,5,5);
    painter->setBrush(Qt::blue);
    painter->drawEllipse(QPointF(begin_midPoint.x() * direct + end_midPoint.x() * (direct ^ 1),begin_midPoint.y() * (direct ^ 1) + end_midPoint.y() * direct),5,5);
    painter->drawEllipse(QPointF(begin_midPoint.x() * (direct ^ 1) + end_midPoint.x() * direct,begin_midPoint.y() * direct + end_midPoint.y() * (direct ^ 1)),5,5);

}
//判断是否可以调整，目前只限制了中间调整点的调整范围
int DPolyLineItem::checkModi(int type,QPointF p)
{
    //int direct = getPaintDirection();
    QPointF st_temp_offset = QPointF(beginPoint.x() + st_x_offset, beginPoint.y() + st_y_offset);
    QPointF ed_temp_offset = QPointF(endPoint.x() + ed_x_offset, endPoint.y() + ed_y_offset);

    if(type == 0) {
        if(st_x_offset < 0 && p.x() > st_temp_offset.x()) return 0;
        if(st_x_offset > 0 && p.x() < st_temp_offset.x()) return 0;
        if(ed_x_offset < 0 && p.x() > ed_temp_offset.x()) return 0;
        if(ed_x_offset > 0 && p.x() < ed_temp_offset.x()) return 0;
    } else if(type == 1) {
        if(st_y_offset < 0 && p.y() > st_temp_offset.y()) return 0;
        if(st_y_offset > 0 && p.y() < st_temp_offset.y()) return 0;
        if(ed_y_offset < 0 && p.y() > ed_temp_offset.y()) return 0;
        if(ed_y_offset > 0 && p.y() < ed_temp_offset.y()) return 0;
    } else if(type == 3) {

    }
    return 1;
}

void DPolyLineItem::modiToPoint(QPointF p, int id)
{
    switch(id)
    {
    case 0:
        if(begin_midPoint.x() == end_midPoint.x()) {
            if(!checkModi(0,p)) return;
            begin_midPoint = QPointF(p.x(),begin_midPoint.y());
            end_midPoint = QPointF(p.x(),end_midPoint.y());
        } else if(begin_midPoint.y() == end_midPoint.y()) {
            if(!checkModi(1,p)) return;
            begin_midPoint = QPointF(begin_midPoint.x(),p.y());
            end_midPoint = QPointF(end_midPoint.x(),p.y());
        }
        modi_pos.insert(0,p);
        updateModiPoint();
        break;
    case 1:
        if(begin_midPoint.x() == end_midPoint.x()) {
            //if(!checkModi(0,p)) return;
            st_y_offset = p.y() - beginPoint.y();
            begin_midPoint = QPointF(end_midPoint.x(),p.y());
        } else if(begin_midPoint.y() == end_midPoint.y()) {
            //if(!checkModi(1,p)) return;
            st_x_offset = p.x() - beginPoint.x();
            begin_midPoint = QPointF(p.x(),end_midPoint.y());
        }
        modi_pos.insert(1,p);
        updateModiPoint();
        break;
    case 2:
        if(begin_midPoint.x() == end_midPoint.x()) {
            //if(!checkModi(0,p)) return;
            ed_y_offset = p.y() - endPoint.y();
            end_midPoint = QPointF(begin_midPoint.x(),p.y());
        } else if(begin_midPoint.y() == end_midPoint.y()) {
            //if(!checkModi(1,p)) return;
            ed_x_offset = p.x() - endPoint.x();
            end_midPoint = QPointF(p.x(),begin_midPoint.y());
        }
        modi_pos.insert(2,p);
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
    updateBeginMidPoint(line_type);
    updateEndMidPoint(line_type);
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
        mid_x_shift = beginPoint.x() + st_x_offset;
        mid_y_shift = endPoint.y() + ed_y_offset;
        break;
    case 2:
        mid_x_shift = endPoint.x() + ed_x_offset;
        mid_y_shift = beginPoint.y() + st_y_offset;
        break;
    default:
        mid_x_shift = midPoint.x();
        mid_y_shift = midPoint.y();
        break;
    }
    return;
}

void DPolyLineItem::updateModiPoint()
{
    if(line_type == 5) { //line_type == 1 || line_type == 2
        modis_num = 0;
        modis.resize(0);
    } else {
        modis_num = 1;
        modis.resize(1);
        modis[0] = (begin_midPoint + end_midPoint) / 2;
        if(begin_midPoint.x() != beginPoint.x() && begin_midPoint.y() != beginPoint.y()) {
            //(abs(st_x_offset) > 1e-6 || abs(st_y_offset) > 1e-6)
            modis_num++;
            modis.resize(modis_num);
            modis[modis_num - 1] = QPointF((beginPoint.x() + st_x_offset + begin_midPoint.x()) / 2,
                                           (beginPoint.y() + st_y_offset + begin_midPoint.y()) / 2);
        }
        if(end_midPoint.x() != endPoint.x() && end_midPoint.y() != endPoint.y()) {
            //(abs(ed_x_offset) > 1e-6 || abs(ed_y_offset) > 1e-6)
            modis_num++;
            modis.resize(modis_num);
            modis[modis_num - 1] = QPointF((endPoint.x() + ed_x_offset + end_midPoint.x()) / 2,
                                           (endPoint.y() + ed_y_offset + end_midPoint.y()) / 2);
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

int DPolyLineItem::getPaintDirection() const
{
    if(!line_type || line_type == 3) { //
        if(abs(endPoint.x() - beginPoint.x()) > abs(endPoint.y() - beginPoint.y())) return 0;
        else return 1;
    } else {
        if(abs(endPoint.x() - beginPoint.x()) > abs(endPoint.y() - beginPoint.y())) return 1;
        else return 0;
    }
}

//上右下左1234 注意画的时候图形不能太小，要不然会判断错误
int DPolyLineItem::getCollideDirection(QRectF item,QPointF center,QPointF point)
{
    int item_direct = 0;
    double min_dis = 0x3f3f3f3f, temp = 0;
    if(abs(center.x() + item.left() - point.x()) < 40) {
        temp = abs(center.x() + item.left() - point.x());
        if(temp < min_dis) min_dis = temp,item_direct = 4;
    }
    else if(abs(center.x() + item.right() - point.x()) < 40) {
        temp = abs(center.x() + item.right() - point.x());
        if(temp < min_dis) min_dis = temp,item_direct = 2;
    }
    else if(abs(center.y() + item.top() - point.y()) < 40) {
        temp = abs(center.y() + item.top() - point.y());
        if(temp < min_dis) min_dis = temp,item_direct = 1;
    }
    else if(abs(center.y() + item.bottom() - point.y()) < 40) {
        temp = abs(center.y() + item.bottom() - point.y());
        if(temp < min_dis) min_dis = temp,item_direct = 3;
    }
    return item_direct;
}
//相对位置 1234右上开始顺时针
int DPolyLineItem::getRelativePosition(QPointF cur,QPointF another)
{
    if(another.x() > cur.x())
        return (another.y() > cur.y()) ? 2 : 1;
    else return (another.y() > cur.y()) ? 3 : 4;
}

void DPolyLineItem::updateBeginMidPoint(int type)
{
    int direct = getPaintDirection();
    switch(type)
    {
    case 4:
        begin_midPoint = QPointF((beginPoint.x() + st_x_offset) * direct + mid_x_shift * (direct ^ 1),
                                 (beginPoint.y() + st_y_offset) * (direct ^ 1) + mid_y_shift * direct);

        break;
    default:
        begin_midPoint = QPointF((beginPoint.x() + st_x_offset) * direct + mid_x_shift * (direct ^ 1),
                                 (beginPoint.y() + st_y_offset) * (direct ^ 1) + mid_y_shift * direct);
        break;
    }
}
void DPolyLineItem::updateEndMidPoint(int type)
{
    int direct = getPaintDirection();
    switch(type)
    {
    case 4:
        end_midPoint = QPointF((endPoint.x() + ed_x_offset) * direct + mid_x_shift * (direct ^ 1),
                               (endPoint.y() + ed_y_offset) * (direct ^ 1) + mid_y_shift * direct);

        break;
    default:
        end_midPoint = QPointF((endPoint.x() + ed_x_offset) * direct + mid_x_shift * (direct ^ 1),
                               (endPoint.y() + ed_y_offset) * (direct ^ 1) + mid_y_shift * direct);
        break;
    }
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
        begin_item_direct = getCollideDirection(st_item,st_center,beginPoint);
    }
    if(endMag) {
        ed_item = endMag->parent->boundingRect();
        ed_center = endMag->parent->pos();
        end_item_direct = getCollideDirection(ed_item,ed_center,endPoint);
    }
    qDebug() << "directions:" << begin_item_direct << "," << end_item_direct;
    updateOffsets(begin_item_direct,end_item_direct);
    return;
}

//得到图形对应的点 右上顺时针1234
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
    qDebug() << "bound:" << bound.left() << bound.right() << bound.top() << bound.bottom();
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
    int rect_type = getPolyLineDirection();
    int temp_relative_pos;
    int begin_line_type = 0,end_line_type = 0;
    QPointF st_midPoint = {0,0},ed_midPoint = {0,0};
    switch (st_dir) {
    case 0:
        st_x_offset = 0;
        st_y_offset = 0;
        begin_line_type = 0;
        break;
    case 1:
        begin_line_type = 0;
        st_y_offset = -record_dist;
        st_x_offset = 0;
        temp_relative_pos = getRelativePosition(QPointF(beginPoint.x() + st_x_offset, beginPoint.y() + st_y_offset),endPoint);
        if(!rect_type) {
            if(temp_relative_pos == 1 || temp_relative_pos == 4) {
                begin_line_type = 1;
                //st_midPoint = QPointF(beginPoint.x(),endPoint.y());
            }
            st_midPoint = getBoundingPoint(0,temp_relative_pos);
        } else {
            if(temp_relative_pos == 1 || temp_relative_pos == 4) {
                st_midPoint = getBoundingPoint(0,temp_relative_pos);
            }
            else if(temp_relative_pos == 2) {
                begin_line_type = 3;
                st_midPoint = getBoundingPoint(0,1);
            } else if(temp_relative_pos == 3) {
                begin_line_type = 3;
                st_midPoint = getBoundingPoint(0,4);
            }
        }
        break;
    case 2:
        begin_line_type = 0;
        st_x_offset = record_dist;
        st_y_offset = 0;
        temp_relative_pos = getRelativePosition(QPointF(beginPoint.x() + st_x_offset, beginPoint.y() + st_y_offset),endPoint);
        if(rect_type) {
            if(temp_relative_pos == 1 || temp_relative_pos == 2) {
                begin_line_type = 2;
                //st_midPoint = QPointF(endPoint.x(),beginPoint.y());
            }
            st_midPoint = getBoundingPoint(0,temp_relative_pos);
        } else {
            if(temp_relative_pos == 1 || temp_relative_pos == 2) {
                st_midPoint = getBoundingPoint(0,temp_relative_pos);
            }
            else if(temp_relative_pos == 3) {
                begin_line_type = 3;
                st_midPoint = getBoundingPoint(0,2);
            } else if(temp_relative_pos == 4) {
                begin_line_type = 3;
                st_midPoint = getBoundingPoint(0,1);
            }
        }
        break;
    case 3:
        begin_line_type = 0;
        st_y_offset = record_dist;
        st_x_offset = 0;
        temp_relative_pos = getRelativePosition(QPointF(beginPoint.x() + st_x_offset, beginPoint.y() + st_y_offset),endPoint);
        if(!rect_type) {
            if(temp_relative_pos == 2 || temp_relative_pos == 3) {
                begin_line_type = 1;
                //st_midPoint = QPointF(beginPoint.x(),endPoint.y());
            }
            st_midPoint = getBoundingPoint(0,temp_relative_pos);
        } else {
            if(temp_relative_pos == 2 || temp_relative_pos == 3) {
                st_midPoint = getBoundingPoint(0,temp_relative_pos);
            }
            else if(temp_relative_pos == 1) {
                begin_line_type = 3;
                st_midPoint = getBoundingPoint(0,2);
            } else if(temp_relative_pos == 4) {
                begin_line_type = 3;
                st_midPoint = getBoundingPoint(0,3);
            }
        }
        break;
    case 4:
        begin_line_type = 0;
        st_x_offset = -record_dist;
        st_y_offset = 0;
        temp_relative_pos = getRelativePosition(QPointF(beginPoint.x() + st_x_offset, beginPoint.y() + st_y_offset),endPoint);
        if(rect_type) {
            if(temp_relative_pos == 3 || temp_relative_pos == 4) {
                begin_line_type = 2;
                //st_midPoint = QPointF(endPoint.x(),beginPoint.y());
            }
            st_midPoint = getBoundingPoint(0,temp_relative_pos);
        } else {
            if(temp_relative_pos == 3 || temp_relative_pos == 4) {
                st_midPoint = getBoundingPoint(0,temp_relative_pos);
            }
            else if(temp_relative_pos == 1) {
                begin_line_type = 3;
                st_midPoint = getBoundingPoint(0,4);
            } else if(temp_relative_pos == 2) {
                begin_line_type = 3;
                st_midPoint = getBoundingPoint(0,3);
            }
        }
        break;
    }

    switch (ed_dir) {
    case 0:
        ed_x_offset = 0;
        ed_y_offset = 0;
        end_line_type = 0;
        break;
    case 1:
        end_line_type = 0;
        ed_y_offset = -record_dist;
        ed_x_offset = 0;
        temp_relative_pos = getRelativePosition(QPointF(endPoint.x() + ed_x_offset, endPoint.y() + ed_y_offset),beginPoint);
        if(!rect_type) {
            if(temp_relative_pos == 1 || temp_relative_pos == 4) {
                end_line_type = 2;
                //ed_midPoint = QPointF(endPoint.x(),beginPoint.y());
            }
            ed_midPoint = getBoundingPoint(1,temp_relative_pos);
        } else {
            if(temp_relative_pos == 1 || temp_relative_pos == 4) {
                ed_midPoint = getBoundingPoint(1,temp_relative_pos);
            }
            else if(temp_relative_pos == 2) {
                end_line_type = 3;
                ed_midPoint = getBoundingPoint(1,1);
            } else if(temp_relative_pos == 3) {
                end_line_type = 3;
                ed_midPoint = getBoundingPoint(1,4);
            }
        }
        break;
    case 2:
        end_line_type = 0;
        ed_x_offset = record_dist;
        ed_y_offset = 0;
        temp_relative_pos = getRelativePosition(QPointF(endPoint.x() + ed_x_offset, endPoint.y() + ed_y_offset),beginPoint);
        if(rect_type) {
            if(temp_relative_pos == 1 || temp_relative_pos == 2) {
                end_line_type = 1;
                //ed_midPoint = QPointF(beginPoint.x(),endPoint.y());
            }
            ed_midPoint = getBoundingPoint(1,temp_relative_pos);
        } else {
            if(temp_relative_pos == 1 || temp_relative_pos == 2) {
                ed_midPoint = getBoundingPoint(1,temp_relative_pos);
            }
            else if(temp_relative_pos == 3) {
                end_line_type = 3;
                ed_midPoint = getBoundingPoint(1,2);
            } else if(temp_relative_pos == 4) {
                end_line_type = 3;
                ed_midPoint = getBoundingPoint(1,1);
            }
        }
        break;
    case 3:
        end_line_type = 0;
        ed_y_offset = record_dist;
        ed_x_offset = 0;
        temp_relative_pos = getRelativePosition(QPointF(endPoint.x() + ed_x_offset, endPoint.y() + ed_y_offset),beginPoint);
        if(!rect_type) {
            if(temp_relative_pos == 2 || temp_relative_pos == 3) {
                end_line_type = 2;
                //ed_midPoint = QPointF(endPoint.x(),beginPoint.y());
            }
            ed_midPoint = getBoundingPoint(1,temp_relative_pos);
        } else {
            if(temp_relative_pos == 2 || temp_relative_pos == 3) {
                ed_midPoint = getBoundingPoint(1,temp_relative_pos);
            }
            else if(temp_relative_pos == 4) {
                end_line_type = 3;
                ed_midPoint = getBoundingPoint(1,3);
            } else if(temp_relative_pos == 1) {
                end_line_type = 3;
                ed_midPoint = getBoundingPoint(1,2);
            }
        }
        break;
    case 4:
        end_line_type = 0;
        ed_x_offset = -record_dist;
        ed_y_offset = 0;
        temp_relative_pos = getRelativePosition(QPointF(endPoint.x() + ed_x_offset, endPoint.y() + ed_y_offset),beginPoint);
        if(rect_type) {
            if(temp_relative_pos == 3 || temp_relative_pos == 4) {
                end_line_type = 1;
                //ed_midPoint = QPointF(beginPoint.x(),endPoint.y());
            }
            ed_midPoint = getBoundingPoint(1,temp_relative_pos);
        } else {
            if(temp_relative_pos == 3 || temp_relative_pos == 4) {
                ed_midPoint = getBoundingPoint(1,temp_relative_pos);
            }
            else if(temp_relative_pos == 1) {
                end_line_type = 3;
                ed_midPoint = getBoundingPoint(1,4);
            } else if(temp_relative_pos == 2) {
                end_line_type = 3;
                ed_midPoint = getBoundingPoint(1,3);
            }
        }
        break;
    }
    qDebug() << "begin_line_type:" <<begin_line_type << "end_line_type:" << end_line_type;
    qDebug() << "st_midPoint:" << st_midPoint << "ed_midPoint:" << ed_midPoint;
    if(!begin_line_type && !end_line_type) {
        line_type = 0;
    } else if(!end_line_type && begin_line_type) {
        line_type = begin_line_type;
        midPoint = st_midPoint;
    } else if(!begin_line_type && end_line_type) {
        line_type = end_line_type;
        midPoint = ed_midPoint;
    } else {
        line_type = 4;
        //midPoint = (st_midPoint + ed_midPoint) / 2;
        midPoint = (beginPoint + endPoint) / 2;
        //begin_midPoint = st_midPoint;
        //end_midPoint = ed_midPoint;
    }
    qDebug() << "line_type:" << line_type << "midPoint:" << midPoint;
}
