#pragma once

#include "dpolygonbase.h"

class DTriItem : public DPolygonBase
{
public:
	enum { Type = DTriItemType };
	DTriItem(QGraphicsItem *parent = nullptr);
	DTriItem(qreal w, qreal h, QGraphicsItem *parent = nullptr);

public:
	int type() const override { return Type; }
	virtual void modiToPoint(QPointF p, int id) override;

protected:
	void updateMagPoint() override;
	void updateModiPoint() override;

public:
    /**
     * @brief serialize
     * @param out
     * 序列化：DPolygonBase
     */
    void serialize(QDataStream &out) const;

    void deserialize(QDataStream &in);
};

