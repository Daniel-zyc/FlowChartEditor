#include "dfmultidocitem.h"
#include "../../magpoint.h"

DFMultiDocItem::DFMultiDocItem(QGraphicsItem *parent)
	: DFMultiDocItem(minRectSize, minRectSize, parent) {}

DFMultiDocItem::DFMultiDocItem(qreal w, qreal h, QGraphicsItem *parent)
    : DShapeBase("", parent)
{
    path.setFillRule(Qt::WindingFill);
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
    temp=path;
    path.translate(0.2*rect.width(), -0.2*rect.height());
    temp.addPath(path);
    painter->drawPath(path);
    path.translate(-0.1*rect.width(), 0.1*rect.height());
    temp.addPath(path);
    painter->drawPath(path);
    path.translate(-0.1*rect.width(), 0.1*rect.height());
    temp.addPath(path);
    painter->drawPath(path);
}

QRectF DFMultiDocItem::sizeRect() const
{
    return rect;
}

QPainterPath DFMultiDocItem::shapeNormal() const
{
    return temp;
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

    path.moveTo(first_rect.bottomRight() - QPointF(0, h / 4));
    path.lineTo(first_rect.topRight());
    path.lineTo(first_rect.topLeft());
    path.lineTo(first_rect.bottomLeft().x(), first_rect.bottomLeft().y() - h / 10);
    // 设置贝塞尔曲线的四个控制点
    QPointF startPoint(first_rect.bottomLeft().x(), first_rect.bottomLeft().y() - h / 10);
    QPointF controlPoint1(first_rect.bottomLeft().x() + w / 3 , first_rect.bottomLeft().y() + h / 7);
    QPointF controlPoint2(first_rect.bottomLeft().x() + w / 4 * 3, first_rect.bottomLeft().y() - h / 3);
    QPointF endPoint(first_rect.bottomRight().x(), first_rect.bottomRight().y() - h / 4);
    // 绘制三次贝塞尔曲线
    path.cubicTo(controlPoint1, controlPoint2, endPoint);


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
