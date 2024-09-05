#pragma once

#include "../../global.h"
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
    int beginArrowType = DConst::NONE, endArrowType = DConst::NONE;

	virtual int checkInterPoint(QPointF p) const override;
	virtual int setInterPoint(QPointF p) override;
	virtual void interToPoint(QPointF p, MagPoint *mp = nullptr) override;

	virtual void setInsertItem() override;

	virtual void linkBeginUpdate(MagPoint *mp);
	virtual void linkEndUpdate(MagPoint *mp);

	virtual void linkBegin(MagPoint *mp);
	virtual void linkEnd(MagPoint *mp);

	virtual void unlinkBeginUpdate();
	virtual void unlinkEndUpdate();
	virtual void unlinkMagUpdate(MagPoint *mp);

	virtual void unlinkBegin();
	virtual void unlinkEnd();

	virtual void updatePosition();
	virtual void updateSizePoint();

	virtual void setBeginArrowType(int type);
	virtual void setEndArrowType(int type);
	virtual qreal getAngle(const QPointF &beginPoint, const QPointF &endPoint);
    virtual void drawEndArrow(QPainter *painter, double angle, const QPointF &endPoint, int arrowType);
    virtual void drawBeginArrow(QPainter *painter, double angle, const QPointF &endPoint, int arrowType);

	virtual void setBeginPoint(QPointF p);
	virtual void setEndPoint(QPointF p);

    // 获取某个磁吸点的类型
    int magType(MagPoint *mag);
    // 判断是否有回环
    bool ifHasRound();
    // 判断是否与某一个abstractBase相连
    bool ifLinkedWith(DAbstractBase *item);
    // 判断是否与某一个line连接同一个magPoint
    bool ifLinedSameMag(DLineBase *line);

	void checkAutoUnlinkLine();

protected:
	virtual void paintSelected(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	virtual QPainterPath shapeSelected() const override;
	virtual QPainterPath shapeShowMaged() const override;

	virtual void sizeToPoint(QPointF p, int id, MagPoint *mp = nullptr) override;

	//==========================================================================
	virtual void updateLine() = 0;
	//==========================================================================

	// 检测图形变化，并调整图形
	// virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

protected:
	QPointF beginPoint = QPoint(0, 0), endPoint = QPointF(0, 0);
	MagPoint *beginMag = nullptr, *endMag = nullptr;

private:
	int interactType = DConst::NONE;

public:
	/**
	 * @brief serialize
	 * @param out
	 * 序列化：父类DAbstractBase序列化 -> beginPoint -> endPoint -> beginArrowType -> endArrowType
	 */
	void serialize(QDataStream &out, const QGraphicsItem* fa = nullptr) const override;
	bool deserialize(QDataStream &in, QGraphicsItem* fa = nullptr) override;
};

