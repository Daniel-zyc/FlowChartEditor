#pragma once

#include "../base/dpolygonbase.h"

class DFOffPageItem : public DPolygonBase
{
public:
    enum { Type = DFOffPageItemType };
    DFOffPageItem(QGraphicsItem *parent = nullptr);
    DFOffPageItem(qreal w, qreal h, QGraphicsItem* parent = nullptr);

public:
    int type() const override { return Type; }

    void modiToPoint(QPointF, int id) override;

    void updateMagPoint() override;
    void updateModiPoint() override;

public:
    /**
     * @brief serialize
     * @param out
     * 序列化：DShapeBase -> topWidth -> bottomWidth -> height -> c -> rect
     */
    void serialize(QDataStream &out, const QGraphicsItem* fa) const override;
    bool deserialize(QDataStream &in, QGraphicsItem* fa) override;
};
