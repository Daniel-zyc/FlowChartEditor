#pragma once

#include "global.h"

#include <QGraphicsItem>
#include <QPointF>

class DLineBase;

// 磁吸点类，作为 DAbstractBase 的辅助类
// 创建的时候一定要指定父图形项
// 使用的时候要谨慎使用 拷贝构造函数 MagPoint(const MagPoint& other)
class MagPoint
{
public:
	MagPoint(QGraphicsItem* parent);
	// 指定在父图形项坐标系的坐标点
	MagPoint(const QPointF &p, QGraphicsItem* parent);
	// 指定在父图形项坐标系的坐标点
	MagPoint(qreal x, qreal y, QGraphicsItem* parent);

	~MagPoint();

	// 更新与其相连的所有 DLineBase 的位置
	void updateLines() const;

	// 添加线条指针
	void addLine(DLineBase* line);
	// 删除线条指针
	void deleteLine(DLineBase* line);

	// 删除所有线条
	void deleteAllLines();

	// 将自己在父图形项中的坐标映射到 item 的坐标系
	QPointF mapToItem(QGraphicsItem *item);

	// pos 表示在父图形项坐标系中的坐标
	QPointF pos = {0, 0};
	// parent 指向挂载到的父图形项
	QGraphicsItem *parent = nullptr;
	// lines 存储与其相连的 DLineBase 的指针
	QList<DLineBase*> *lines = nullptr;

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
    void deserialize(QDataStream &in);

    void linkLine(DLineBase* line);
};

