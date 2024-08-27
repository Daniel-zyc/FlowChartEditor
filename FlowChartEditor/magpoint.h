#pragma once

#include "dlinebase.h"
#include "dshapebase.h"

#include <QtCore>
#include <QGraphicsItem>

class DLineBase;
class DShapeBase;

// 磁吸点类，作为 DShapeBase 的辅助类
class MagPoint
{
public:
	MagPoint(DShapeBase* parent) : parent(parent) {}
	MagPoint(qreal x, qreal y, DShapeBase* parent) : x(x), y(y), parent(parent) {}

	~MagPoint() = default;

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
	qreal x = 0, y = 0;
	// parent 指向挂载到的 DShapeBase
	DShapeBase* parent;
	// lines 存储与其相连的 DLineBase 的指针
	QList<DLineBase*> lines = QList<DLineBase*>();
};

