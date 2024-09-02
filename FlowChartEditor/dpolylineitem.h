#ifndef DPOLYLINEITEM_H
#define DPOLYLINEITEM_H

#include "global.h"
#include "dlinebase.h"
#include "magpoint.h"
#include "dshapebase.h"
#include <QtMath>

class DPolyLineItem : public DLineBase
{
public:
    DPolyLineItem(QGraphicsItem * parent = nullptr);
    DPolyLineItem(QPointF begin, QPointF end, QGraphicsItem *parent = nullptr);

    ~DPolyLineItem() = default;

    int type() const override { return Type; }

    QRectF boundingRect() const override;

protected:
    //获取上下左右的最大框架
    QPointF getMinPoint(QVector<QPointF> points) const;
    QPointF getMaxPoint(QVector<QPointF> points) const;

    void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void modiToPoint(QPointF p, int id) override;

    QPainterPath shapeNormal() const override;

    void updateLine() override;
    //调整中点位置
    void updateMidPoint(int type);
    //获取调整点数量及位置
    void updateModiPoint();
    //获取折线长短边方向
    int getPolyLineDirection() const;
    //获取碰撞方向
    int getCollideDirection(QRectF item,QPointF center);
    //获取另一点与这个点的相对位置
    int getRelativePosition(QPointF cur,QPointF another);
    //获取线头连线的图形是否有碰撞
    //int HasItemCollision(QRectF st_item,QRectF ed_item);
    //调整折线形
    void updatePolyLineType();
    QPointF getBoundingPoint(int point,int type);
    void updateOffsets(int st_dir,int ed_dir);


private:
    //起终点位置偏移量
    double st_x_offset = 0, st_y_offset = 0;
    double ed_x_offset = 0, ed_y_offset = 0;
    //中点位置
    double mid_x_shift = 0, mid_y_shift = 0;
    //中点位置记录；
    QPointF midPoint = {0,0};
    int modis_num = 1;
    //折线类型
    int line_type = 0;
};

#endif // DPOLYLINEITEM_H
