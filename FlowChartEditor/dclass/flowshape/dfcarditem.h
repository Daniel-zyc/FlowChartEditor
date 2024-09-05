#pragma once

#include "../base/dpolygonbase.h"

class DFCardItem : public DPolygonBase
{
public:
    enum { Type = DFCardItemType };
    DFCardItem(QGraphicsItem *parent = nullptr);
    DFCardItem(qreal w, qreal h, QGraphicsItem* parent = nullptr);

public:
    int type() const override { return Type; }

    void modiToPoint(QPointF, int id) override;

    void updateMagPoint() override;
    void updateModiPoint() override;

public:
    void serialize(QDataStream &out, const QGraphicsItem* fa) const override;
    bool deserialize(QDataStream &in, QGraphicsItem* fa) override;
};
