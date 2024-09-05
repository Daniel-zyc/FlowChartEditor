#pragma once

#include "../../global.h"

#include <QPen>
#include <QBrush>
#include <QVector>
#include <QPointF>
#include <QPainter>
#include <QGraphicsItem>

class MagPoint;

// 一个抽象的基类，DShapeBase 和 DLineBase 都继承于此
// 本基类有磁吸点，但是实际上是由 DShapeBase 会使用磁吸点，DLineBase 没有磁吸点
class DAbstractBase : public QAbstractGraphicsShapeItem
{
public:
	enum { Type = DAbstractBaseType };
	DAbstractBase(QGraphicsItem *parent = nullptr);
	~DAbstractBase();
	
public:
	virtual int type() const override { return Type; }

	// 获取图形的碰撞路径，根据图形的状态取 shapeNormal, shapeSelected, shapeShowMaged 的并集
	virtual QPainterPath shape() const override;
	// 绘制图形项，绘制顺序为 paintShape，paintSelected(被选中), paintMagPoint(显示磁吸点）
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

	// 图形的类型判断，含义与 global 中相同
	virtual bool isAbstract();
	virtual bool isShape();
	virtual bool isFLowChartShape();
	virtual bool isLine();
	virtual bool isText();

	// 设置大小和旋转，根据图形是否可以进行大小变换和旋转变换来调整
	virtual void setScale(qreal scl);
	virtual void setRotation(qreal deg);

protected:
	// 被选中下多出的碰撞范围
	virtual QPainterPath shapeSelected() const;
	// 显示磁吸点时多出的碰撞范围
	virtual QPainterPath shapeShowMaged() const;

	//==========================================================================
	// 正常情况下的碰撞范围
	virtual QPainterPath shapeNormal() const = 0;
	//==========================================================================

protected:
	// 绘制被选中时额外需要绘制的部分
	virtual void paintSelected(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
	// 绘制调整点
	virtual void paintModiPoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
	// 绘制大小点
	virtual void paintSizePoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
	// 绘制磁吸点
	virtual void paintMagPoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

	//==========================================================================
	// 绘制图形本身
	virtual void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) = 0;
	//==========================================================================

public:
	// 设置是否显示磁吸点
	virtual void setShowMagPoint(bool show = true);
	// 检查 scene 坐标系下 p 是否与某个磁吸点碰撞，使用 CollideRadius 进行检测
	virtual bool checkMagPoint(QPointF p) const;
	// 获取与 scene 坐标系下的 p 点碰撞的最近的磁吸点的指针
	virtual MagPoint* getMagPoint(QPointF p);

	//==========================================================================
	// 检查 scene 坐标系下 p 是否与某个交互点碰撞，并返回交互类型
	virtual int checkInterPoint(QPointF p) const = 0;
	// 根据 scene 坐标系下 p 点设置当前交互状态和交互点，并返回交互类型
	virtual int setInterPoint(QPointF p) = 0;
	// 依据之前设置的状态，调整交互点的参数和图形的状态，其中 p 为 scene 坐标系下
	virtual void interToPoint(QPointF p, MagPoint *mp = nullptr) = 0;
	//==========================================================================

	// 将图形设置为插入状态，即将当前交互点设置为图形的右下，或者线条的结尾端点
	virtual void setInsertingItem() = 0;

	// 更新所有连接的线条
	virtual void updateAllLinkLines();

	// 断开所有连接的线条
	virtual void unLinkAllLines();

    // 获取图形连线的类型，in，out，none
    virtual std::tuple<int,int,int> getLinedArrowType();

protected:
	// 检查是否与调整点碰撞，p 为 item 坐标系下
	virtual int checkModiPoint(QPointF p) const;
	// 设置与 p 碰撞的最近的调整点，p 为 item 坐标系下
	virtual bool setModiPoint(QPointF p);

	// 检查是否与大小点碰撞，p 为 item 坐标系下
	virtual int checkSizePoint(QPointF p) const;
	// 设置与 p 碰撞的最近的大小点，p 为 item 坐标系下
	virtual bool setSizePoint(QPointF p);

	// 准备将选中的调整点设置到 p 位置，p 为 item 坐标系下
	virtual void modiToPointPre(QPointF P);
	// 准备将选中的大小点设置到 p 位置，p 为 item 坐标系下
	virtual void sizeToPointPre(QPointF p, MagPoint *mp = nullptr);
	
	//==========================================================================
	// 将选中的调整点 id，设置到 p 位置，p 为 item 坐标系下
	virtual void modiToPoint(QPointF p, int id) = 0;
	// 将选中的大小点 id，设置到 p 位置，p 为 item 坐标系下
	virtual void sizeToPoint(QPointF p, int id, MagPoint *mp = nullptr) = 0;
	//==========================================================================

	// 设置图形能否进行旋转或大小调整，会调用图形的 update 函数
	virtual void setRotateable(bool state);
	virtual void setScaleable(bool state);

	// 检测图形的状态是否发生改变，用于撤销重做
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

public:
	bool isScaleable = true;
	bool isRotateable = true;

protected:
	// 图形所具有的磁吸点
	QList<MagPoint*> *mags = nullptr;
	// 图形的调整和大小点
	QList<QPointF> modis = QList<QPointF>();
	QList<QPointF> sizes = QList<QPointF>();
	// 选中的调整点、大小点 Id
	int modiPointId = -1, sizePointId = -1;
	// 是否显示磁吸点
	bool showMagPoint = false;

public:
	// 对图形进行序列化，会输出图形的位置/旋转/大小/Z值，笔刷/画笔
	// 此处不输出磁吸点信息
	virtual void serialize(QDataStream &out, const QGraphicsItem* fa = nullptr) const;
	virtual bool deserialize(QDataStream &in, QGraphicsItem* fa = nullptr);
};

