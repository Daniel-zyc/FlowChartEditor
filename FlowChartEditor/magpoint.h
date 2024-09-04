#pragma once

#include <QGraphicsItem>
#include <QPointF>

class DShapeBase;
class DLineBase;

// 磁吸点类，作为 DAbstractBase 的辅助类
// 创建的时候一定要指定父图形项
// 使用的时候要谨慎使用 拷贝构造函数 MagPoint(const MagPoint& other)
class MagPoint
{
public:
	// 构造函数，需要指明挂载到的父图形项
    MagPoint(DShapeBase* parent);
	// 指定在父图形项坐标系的坐标点
    MagPoint(const QPointF &p, DShapeBase* parent);
	// 指定在父图形项坐标系的坐标点
    MagPoint(qreal x, qreal y, DShapeBase* parent);

	~MagPoint();

	// 更新与其相连的所有 DLineBase 的位置（如果 scenePos 不变则不刷新）
	void updateAllLinkLines();
	// 更新其在画布上的位置，如果卫视发生改变，则刷新相连图形
	void updateScenePos();

	// 添加线条指针
	void addLine(DLineBase* line);
	// 删除线条指针
	void deleteLine(DLineBase* line);

	// 设置在父坐标系下的位置
	void setPos(const QPointF &p);

	// 删除所有线条
	void unlinkAllLines();

	// 将自己在父图形项中的坐标映射到 item 的坐标系
	QPointF mapToItem(QGraphicsItem *item);
	QPointF mapToScene();

	// pos 表示在父图形项坐标系中的坐标
	QPointF pos;
	// 记录上一次刷新时的 scenePos，如果不改变则不刷新
	QPointF scenePos;
	// parent 指向挂载到的父图形项
    DShapeBase *parent = nullptr;
    // 获取连线数量
    int linkedLineNum(){if(lines == nullptr) return 0; else return lines->size();}
    // 获取连线的箭头类型数量,int,out,none
    std::tuple<int,int,int> linkedLienArrowType();
private:
	// lines 存储与其相连的 DLineBase 的指针
	QSet<DLineBase*> *lines = nullptr;

public:
    //=====================================
    /**
     * @brief serialize
     * @param out
     * 序列化：this地址 -> parent地址 -> lines大小 -> lines地址
     */
	void serialize(QDataStream &out) const;
    /**
	 * @brief deserialize
     * @param in
     * 反序列化: -> this地址 -> parent地址 -> lines大小 -> lines地址
	 */
	void deserialize(QDataStream &in, QGraphicsItem* fa);
};

