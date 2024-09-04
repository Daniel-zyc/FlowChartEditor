#pragma once

#include "dpolygonbase.h"

#include <QPainter>
#include <QPolygonF>
#include <QRectF>

class pentagonItem : public DPolygonBase
{
public:
    enum { Type = DDiaItemType };
    pentagonItem(QGraphicsItem *parent = nullptr);
    pentagonItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

public:
    int type() const override { return Type; }

    void modiToPoint(QPointF p, int id) override;

    void updateMagPoint() override;
    void updateModiPoint() override;

public:
    /**
     * @brief serialize
     * @param out
     * 序列化：DShapeBase -> rect
     */
    void serialize(QDataStream &out, const QGraphicsItem* fa) const override;
    bool deserialize(QDataStream &in, QGraphicsItem* fa) override;
};
