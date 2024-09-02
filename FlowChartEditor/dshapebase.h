#pragma once

#include "global.h"
#include "dabstractbase.h"

class DTextItem;

// 图形项的基类，图形项继承于此，文本框也继承于此
// 由于图形项中包含文本框，请注意不要导致初始化时对象的创建无限嵌套
// 所有的图形都应该保证 sizeRect 的中心点为 (0, 0)
class DShapeBase : public DAbstractBase
{
public:
	enum { Type = DShapeBaseType };
	DShapeBase(QGraphicsItem *parent = nullptr);
	DShapeBase(const QString &text, QGraphicsItem *parent = nullptr);
	~DShapeBase() = default;

public:
	virtual int type() const override { return Type; }

	// 重载包围框为 sizeRect() 向四个方向进行范围的扩大
	// 此包围框被用来设置在绘制时的范围，因此比图形本身最紧的包围框要大一圈
	QRectF boundingRect() const override;

	//==========================================================================
	// 绘制图形本身
	virtual void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override = 0;
	//==========================================================================

	// 绘制选中时的图形，顺序为 selectedRect, sizePoint, modiPoint, rotPoint
	virtual void paintSelected(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	// 绘制图形的正常包围框
	virtual void paintSelectRect(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
	// 绘制图形的旋转点，一个点 + 一根线(样式同 SelectedRect)
	virtual void paintRotPoint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

public:
	// 检查是否与某交互点碰撞，p 为 scene 坐标系下
	virtual int checkInterPoint(QPointF p) const override;
	// 设置当前交互点为某交互点，p 为 scene 坐标系下
	virtual int setInterPoint(QPointF p) override;
	// 根据 p 点来对当前交互点进行调整，p 为 scene 坐标系下
	virtual void interToPoint(QPointF p, MagPoint *mp = nullptr) override;

	// 设置为正在插入这个腿型
	virtual void setInsertItem() override;

public:
	//==========================================================================
	// 图形的大小框
	virtual QRectF sizeRect() const = 0;
	//==========================================================================

protected:
	// 检查是否与旋转点碰撞，p 为 item 坐标系下
	virtual bool checkRotPoint(QPointF p) const;
	// 设置当前交互点为旋转点，p 为 item 坐标系下
	virtual bool setRotPoint(QPointF p);

	// 根据 p 和 id 来调整图形的大小和位置，p 为 item 坐标系下，此函数内部会调用 sizeRectUpdated 来进行刷新
	virtual void sizeToPoint(QPointF p, int id, MagPoint *mp = nullptr) override;
	// 根据向量 p 来调整图形角度
	virtual void rotToPoint(QPointF p);
	// 更新图形的大小点和旋转点坐标，如果图形的大小改变需要调用此函数进行刷新
	virtual void sizeRectUpdated();

protected:
	// 被选中时多出的碰撞框，与父类相比多出了旋转点
	virtual QPainterPath shapeSelected() const override;

	//==========================================================================
	// 图形的正常碰撞范围
	virtual QPainterPath shapeNormal() const override = 0;
	// 将图形 sizeRect 设置到与 nrect 大小相同，保证 nrect 中心点为 (0, 0)
	virtual void sizeToRect(QRectF nrect) = 0;
	// 根据 p 点位置和调整点 id 来调整图形样式, p 为 item 坐标系下
	virtual void modiToPoint(QPointF p, int id) override = 0;
	//==========================================================================

	// 检测图形变化，并调整图形
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
	// 辅助根据计算图形的新矩形框
	QRectF getResizeRect(const QPointF &p, int id);

public:
	// 图形自己的文本框
	DTextItem *textItem = nullptr;

protected:
	// 旋转点大小和其中心距离 sizeRect 的距离
	qreal rotPointRadius = 5, rotPointMargin = 30;
	// 旋转点位置
	QPointF rotPoint = {0, 0};
	
private:
	// 当前的交互类型
	int interactType = DConst::NONE;

public:
    /**
     * @brief serialize
     * @param out
     * 序列化：DAbstractBase -> textPtr
     */
	virtual void serialize(QDataStream &out, const QGraphicsItem* fa = nullptr) const override;
	virtual bool deserialize(QDataStream &in, QGraphicsItem* fa = nullptr) override;
};

