#pragma once

#include "../../global.h"
#include "dabstractbase.h"

#include <QGraphicsItem>
#include <QLineF>

class MagPoint;

// 所有线条类的基类，线条定义为具有和磁吸点相连功能的图形
// 且只有一头一尾两个磁吸点
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

	virtual void setInsertingItem() override;

	// 连接磁吸点并刷新线条形态
	virtual void linkBeginUpdate(MagPoint *mp);
	virtual void linkEndUpdate(MagPoint *mp);

	// 连接磁吸点但不刷新线条形态
	virtual void linkBegin(MagPoint *mp);
	virtual void linkEnd(MagPoint *mp);

	//断开磁吸点并刷新线条形态
	virtual void unlinkBeginUpdate();
	virtual void unlinkEndUpdate();
	virtual void unlinkMagUpdate(MagPoint *mp);

	// 断开磁吸点但不刷新线条形态
	virtual void unlinkBegin();
	virtual void unlinkEnd();

	// 刷新线条形态
	virtual void updatePosition();
	// 刷新大小点的位置
	virtual void updateSizePoint();

	// 设置起始/终止点的位置，会调用 SizeToPoint函数来进行线条的刷新
	virtual void setBeginPoint(QPointF p);
	virtual void setEndPoint(QPointF p);

	// 设置线头的类型
	virtual void setBeginArrowType(int type);
	virtual void setEndArrowType(int type);

	// 绘制箭头时，辅助计算角度
	virtual qreal getAngle(const QPointF &beginPoint, const QPointF &endPoint);
	// 辅助绘制箭头函数
	virtual void drawArrow(QPainter *painter, double angle, const QPointF &endPoint, int arrowType);

    // 获取某个磁吸点的类型
    int magType(MagPoint *mag);
    // 判断是否有回环
    bool ifHasRound();
    // 判断是否与某一个abstractBase相连
    bool ifLinkedWith(DAbstractBase *item);
    // 判断是否与某一个line连接同一个magPoint
    bool ifLinedSameMag(DLineBase *line);

	// 检查头尾两个磁吸点是否断开，如果断开则进行断开操作
	// 用来在线条位置发生移动之后，判断是否还应该保持磁吸关系
	void checkAutoUnlinkLine();

protected:
	virtual void paintSelected(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	virtual QPainterPath shapeSelected() const override;
	virtual QPainterPath shapeShowMaged() const override;

	virtual void sizeToPoint(QPointF p, int id, MagPoint *mp = nullptr) override;

	//==========================================================================
	virtual void updateLine() = 0;
	//==========================================================================

public:
	// 起始终止箭头类型，最好使用 set 函数进行设置
	int beginArrowType = DConst::NONE, endArrowType = DConst::NONE;

protected:
	// 起始/终止点的位置，在刷新之后，永远和 beginMag/endMag 匹配
	QPointF beginPoint = QPoint(0, 0), endPoint = QPointF(0, 0);
	// 起始/终止磁吸点
	MagPoint *beginMag = nullptr, *endMag = nullptr;

private:
	// 记录当前的交互状态
	int interactType = DConst::NONE;

public:
	// 线条的序列化，会输出起始/终止位置，起始/终止磁吸点，起始/终止箭头类型
	virtual void serialize(QDataStream &out, const QGraphicsItem* fa = nullptr) const override;
	virtual bool deserialize(QDataStream &in, QGraphicsItem* fa = nullptr) override;
};

