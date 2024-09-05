#include "dfmultidocitem.h"
#include "../../magpoint.h"

DFMultiDocItem::DFMultiDocItem(QGraphicsItem *parent)
	: DFMultiDocItem(minRectSize, minRectSize, parent) {}

DFMultiDocItem::DFMultiDocItem(qreal w, qreal h, QGraphicsItem *parent)
    : DShapeBase("", parent)
{
    for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
    rect = QRectF(-w/2, -h/2, w, h);
    updateSizePoint();
    updatePath();
    updateMagPoint();
}

void DFMultiDocItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    painter->setBrush(brush());
    painter->setPen(pen());
    painter->drawPath(path);
}

QRectF DFMultiDocItem::sizeRect() const
{
    return rect;
}

QPainterPath DFMultiDocItem::shapeNormal() const
{
    return path;
}

void DFMultiDocItem::sizeToRect(QRectF nrect)
{
	rect = nrect;
	updateSizePoint();
	updatePath();
	updateMagPoint();
}

void DFMultiDocItem::modiToPoint(QPointF p, int id)
{
    Q_UNUSED(p); Q_UNUSED(id);
    return;
}

void DFMultiDocItem::updatePath()
{
    path.clear();

    qreal ratio = 0.8;
    qreal tot_height = rect.height(),tot_width = rect.width();
    QRectF first_rect = QRectF(rect.topLeft().x(),rect.topLeft().y() + tot_height * (1 - ratio), tot_width * ratio , tot_height * ratio);
    qreal w = first_rect.width();
    qreal h = first_rect.height();

    path.moveTo(first_rect.bottomLeft() - QPointF(0 , h / 10));
    path.lineTo(first_rect.topLeft());
    path.lineTo(first_rect.topRight());
    path.lineTo(first_rect.bottomRight() - QPointF(0, h / 4));
    // 设置贝塞尔曲线的四个控制点
    QPointF startPoint(first_rect.bottomLeft().x(), first_rect.bottomLeft().y() - h / 10);
    QPointF controlPoint1(first_rect.bottomLeft().x() + w / 3 , first_rect.bottomLeft().y() + h / 7);
    QPointF controlPoint2(first_rect.bottomLeft().x() + w / 4 * 3, first_rect.bottomLeft().y() - h / 3);
    QPointF endPoint(first_rect.bottomRight().x(), first_rect.bottomRight().y() - h / 4);
    path.moveTo(startPoint);
    // 绘制三次贝塞尔曲线
    path.cubicTo(controlPoint1, controlPoint2, endPoint);
    // //第二个矩形
    // QRectF second_rect = QRectF(first_rect.topLeft().x() + tot_width * (1 - ratio) / 2 ,first_rect.topLeft().y() - tot_height * (1 - ratio) / 2,w,h);
    // path.moveTo(first_rect.topLeft() + QPointF(tot_width * (1 - ratio) / 2 , 0));
    // path.lineTo(second_rect.topLeft());
    // path.lineTo(second_rect.topRight());
    // path.lineTo(second_rect.bottomRight() - QPointF(0 , h / 4));
    // //画第二条的部分
    // QPainterPath CubicPath2;
    // QPointF offset2 = QPointF(tot_width * (1 - ratio) / 2, - tot_height * (1 - ratio) / 2);
    // CubicPath2.moveTo(startPoint + offset2);
    // CubicPath2.cubicTo(controlPoint1 + offset2, controlPoint2 + offset2, endPoint + offset2);
    // qreal _percent2 = 0.001,_percent3 = 0.001;
    // QPointF curPoint,prePoint = {0,0},curPoint2;
    // while(_percent2 < 1) {
    //     curPoint = CubicPath2.pointAtPercent(_percent2);
    //     if(prePoint.x() != 0 &&(first_rect.right() - prePoint.x()) * (first_rect.right() - curPoint.x()) < 0) break;
    //     prePoint = curPoint;
    //     _percent2 += 0.001;
    // }
    // path.moveTo((prePoint + curPoint) / 2);
    // curPoint2 = curPoint;
    // _percent3 = _percent2;
    // while(_percent2 <= 1) {
    //     curPoint = CubicPath2.pointAtPercent(_percent2);
    //     path.lineTo(curPoint);
    //     _percent2 += 0.001;
    // }
    // //第三个矩形
    // QRectF third_rect = QRectF(first_rect.topLeft().x() + tot_width * (1 - ratio) , rect.topLeft().y(),w,h);
    // path.moveTo(second_rect.topLeft() + QPointF(tot_width * (1 - ratio) / 2 , 0));
    // path.lineTo(third_rect.topLeft());
    // path.lineTo(third_rect.topRight());
    // path.lineTo(third_rect.bottomRight() - QPointF(0 , h / 4));
    // //第三条曲线部分
    // path.moveTo((prePoint + curPoint2) / 2 + offset2);
    // while(_percent3 <= 1) {
    //     curPoint2 = CubicPath2.pointAtPercent(_percent3) + offset2;
    //     path.lineTo(curPoint2);
    //     _percent3 += 0.001;
    // }

    // // 设置贝塞尔曲线的四个控制点
    // QPointF startPoint(first_rect.bottomLeft().x(), first_rect.bottomLeft().y() - h / 10);
    // QPointF controlPoint1(first_rect.bottomLeft().x() + w / 3 , first_rect.bottomLeft().y() + h / 7);
    // QPointF controlPoint2(first_rect.bottomLeft().x() + w / 4 * 3, first_rect.bottomLeft().y() - h / 3);
    // QPointF endPoint(first_rect.bottomRight().x(), first_rect.bottomRight().y() - h / 4);
    // path.moveTo(startPoint);
    // // 绘制三次贝塞尔曲线
    // path.cubicTo(controlPoint1, controlPoint2, endPoint);
    // //画第二条的部分
    // QPainterPath CubicPath2;
    // QPointF offset2 = QPointF(tot_width * (1 - ratio) / 2, - tot_height * (1 - ratio) / 2);
    // CubicPath2.moveTo(startPoint + offset2);
    // CubicPath2.cubicTo(controlPoint1 + offset2, controlPoint2 + offset2, endPoint + offset2);
    // qreal _percent2 = 0.001,_percent3 = 0.001;
    // QPointF curPoint,prePoint = {0,0},curPoint2;
    // while(_percent2 < 1) {
    //     curPoint = CubicPath2.pointAtPercent(_percent2);
    //     if(prePoint.x() != 0 &&(first_rect.right() - prePoint.x()) * (first_rect.right() - curPoint.x()) < 0) break;
    //     prePoint = curPoint;
    //     _percent2 += 0.001;
    // }
    // path.moveTo((prePoint + curPoint) / 2);
    // curPoint2 = curPoint;
    // _percent3 = _percent2;
    // while(_percent2 <= 1) {
    //     curPoint = CubicPath2.pointAtPercent(_percent2);
    //     path.lineTo(curPoint);
    //     _percent2 += 0.001;
    // }
    // //第三条曲线部分
    // path.moveTo((prePoint + curPoint2) / 2 + offset2);
    // while(_percent3 <= 1) {
    //     curPoint2 = CubicPath2.pointAtPercent(_percent3) + offset2;
    //     path.lineTo(curPoint2);
    //     _percent3 += 0.001;
    // }
}

void DFMultiDocItem::updateMagPoint()
{
	(*mags)[0]->setPos({rect.left(), rect.height() / 10});
	(*mags)[1]->setPos({rect.right(), -rect.height() * 0.2});

	(*mags)[2]->setPos({rect.right() - rect.width() * 0.4, rect.top()});
	(*mags)[3]->setPos({rect.left() + rect.width() * 0.4, rect.bottom() - rect.height() * 0.08});
}

//==============================================================================

void DFMultiDocItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
    DShapeBase::serialize(out, fa);

    out << rect;
}

bool DFMultiDocItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
    if(!DShapeBase::deserialize(in, fa)) return false;

    in >> rect;
    updateSizePoint();
    updatePath();
    updateMagPoint();
    return true;
}
