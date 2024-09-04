#ifndef DMULTIDOCITEM_H
#define DMULTIDOCITEM_H

#include "dshapebase.h"

class DMultiDocItem : public DShapeBase
{
public:
    enum { Type = DFDocumentItemType };
    DMultiDocItem(QGraphicsItem *parent = nullptr);
    DMultiDocItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

public:
    int type() const override { return Type; }

    void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF sizeRect() const override;
    QPainterPath shapeNormal() const override;
    void sizeToRect(QRectF nrect) override;
    void modiToPoint(QPointF p, int id) override;

    void updatePath();
    void updateMagPoint();

protected:
    QRectF rect = QRectF(0, 0, 0, 0);
    QPainterPath path;
    //double ratio = 100;

public:
    /**
     * @brief serialize
     * @param out
     * 序列化：DShapeBase -> rect -> radiusx -> radiusy
     */
    void serialize(QDataStream &out, const QGraphicsItem* fa) const override;
    bool deserialize(QDataStream &in, QGraphicsItem* fa) override;
};

#endif // DMULTIDOCITEM_H
