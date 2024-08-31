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
namespace DConst
{
	enum UserTypes
	{
		DAbstractBase = QGraphicsItem::UserType + 1,
		DShapeBaseType = QGraphicsItem::UserType + 1,
		DLineBaseType = QGraphicsItem::UserType + 2,
		DTextBaseType = QGraphicsItem::UserType + 3,
		DPolygonBaseType = QGraphicsItem::UserType + 4,
		DRectItemType = QGraphicsItem::UserType + 5,
		DEllItemType = QGraphicsItem::UserType + 6,
		DRoundRectItemType = QGraphicsItem::UserType + 7,
		DLineItemType = QGraphicsItem::UserType + 8,
		DTextItemType = QGraphicsItem::UserType + 9,
        DTriItemType = QGraphicsItem::UserType + 10,
        DDiaItemType=QGraphicsItem::UserType + 13,
        DTrapItemType=QGraphicsItem::UserType + 14
	};
	
	enum None
	{
		NONE = 0
	};

	enum InteractType
	{
		MODI = 1,
		SIZE = 2,
		ROT = 3
	};

	enum Magnet
	{
		MAG = 4
	};

	enum Orient
	{
		T = 1, B = 2, L = 3, R = 4,
		TL = 5, TR = 6,
		BL = 7, BR = 8
	};

	enum LineOrient
	{
		ST = 1, ED = 2
	};

	enum LineArrowType
	{
		ARROW = 1, OPENARROW = 2,
		DOVETAILARROW = 3,
		DIAMONDARROW = 4, ROUNDARROW = 5
	};

	enum Scene
	{

	};
};
