#pragma once

#include "global.h"
#include "dabstractbase.h"

#include <QGraphicsItem>
#include <QLineF>

class MagPoint;

class DLineBase : public DAbstractBase
{
public:
	enum { Type = DLineBaseType };
	DLineBase(QGraphicsItem *parent = nullptr);
	~DLineBase() = default;

public:
	virtual int type() const override { return Type; }

public:
	virtual int checkInterPoint(QPointF p) const override;
	virtual int setInterPoint(QPointF p) override;
	virtual void interToPoint(QPointF p, MagPoint *mp = nullptr) override;

	virtual void linkBegin(MagPoint *mp);
	virtual void linkEnd(MagPoint *mp);

	virtual void unlinkBegin();
	virtual void unlinkEnd();

	virtual void updatePosition();

protected:
	virtual void paintSelected(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	virtual QPainterPath shapeSelected() const override;
	virtual QPainterPath shapeShowMaged() const override;

	virtual void sizeToPoint(QPointF p, int id, MagPoint *mp = nullptr) override;

	//==========================================================================
	virtual void updateLine() = 0;
	//==========================================================================

protected:
	QPointF beginPoint = QPoint(0, 0), endPoint = QPointF(0, 0);
	MagPoint *beginMag = nullptr, *endMag = nullptr;

	int beginArrowType = DConst::NONE, endArrowType = DConst::NONE;

private:
	int interactType = DConst::NONE;

public:
    /**
     * @brief serialize
     * @param out
     * 序列化：父类DAbstractBase序列化 -> beginPoint -> endPoint -> beginArrowType -> endArrowType
     */
    void serialize(QDataStream &out) const;

    void deserialize(QDataStream &in);
};

