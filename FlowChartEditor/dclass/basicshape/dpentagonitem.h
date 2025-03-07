#pragma once

#include "../base/dpolygonbase.h"

#include <QPainter>
#include <QPolygonF>
#include <QRectF>

// 五边形
class DPentagonItem : public DPolygonBase
{
public:
    enum { Type = DPentagonItemType };
    DPentagonItem(QGraphicsItem *parent = nullptr);
    DPentagonItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

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
