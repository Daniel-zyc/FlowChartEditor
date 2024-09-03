#pragma once

#include "dpolygonbase.h"

class DFManualinputItem : public DPolygonBase
{
public:
    enum { Type = DFManualinputItemType };
    DFManualinputItem(QGraphicsItem *parent = nullptr);
    DFManualinputItem(qreal w, qreal h, QGraphicsItem* parent = nullptr);

public:
    int type() const override { return Type; }

    void modiToPoint(QPointF, int id) override;

    void updateMagPoint() override;
    void updateModiPoint() override;
};

