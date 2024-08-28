#pragma once

#include <QGraphicsItem>
#include <QPointF>

class DLineBase;

// 磁吸点类，作为 DAbstractBase 的辅助类
class MagPoint
{
public:
	MagPoint(QGraphicsItem* parent);
	MagPoint(const QPointF &p, QGraphicsItem* parent);
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

	// 将自己的点坐标映射到 item 坐标系
	QPointF mapToItem(QGraphicsItem *item);

	// x, y 表示在 DShapeBase 中的坐标
	QPointF pos = {0, 0};
	// parent 指向挂载到的 DAbstractBase
	QGraphicsItem *parent = nullptr;
	// lines 存储与其相连的 DLineBase 的指针
	QList<DLineBase*> *lines = nullptr;
};

