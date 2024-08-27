#pragma once

#include <QtCore>
#include <QGraphicsItem>

// 圆周率 PI
const qreal PI = 3.14159265358979323;

// 从角度转换到弧度
qreal degToRad(qreal deg);

// 从弧度转换到角度
qreal radToDeg(qreal rad);

// 将角度规范到 [0, 360]
qreal degMod(qreal deg);

// 本值每个图形都应不同
enum UserTypes
{
	DShapeBaseType = QGraphicsItem::UserType + 1,
	DTextItemType = QGraphicsItem::UserType + 2,
	DLineBaseType = QGraphicsItem::UserType + 3,
	DRectItemType = QGraphicsItem::UserType + 4,
	DEllItemType = QGraphicsItem::UserType + 5,
	DRoundRectItemType = QGraphicsItem::UserType + 6,
	DLineItemType = QGraphicsItem::UserType + 7,
	DPolygonBaseType = QGraphicsItem::UserType + 8
};
