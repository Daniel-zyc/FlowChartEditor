#pragma once

#include <QtCore>
#include <QGraphicsItem>
#include <QBrush>
#include <QPen>

// 工具函数
namespace DTool
{
	// 从角度转换到弧度
	qreal degToRad(qreal deg);

	// 从弧度转换到角度
	qreal radToDeg(qreal rad);

	// 将角度规范到 [0, 360]
	qreal degMod(qreal deg);

	// 两点距离
	qreal dist(const QPointF& p1, const QPointF& p2);

	// 平方
	qreal sq(qreal x);

	bool inCircle(const QPointF& o, qreal r, const QPointF& p);
};

// 大小、磁吸、磁吸碰撞、调整、旋转点半径
constexpr qreal sizePointRadius = 5;
constexpr qreal magPointRadius = 5;
constexpr qreal magPointCollideRadius = 10;
constexpr qreal modiPointRadius = 5;
constexpr qreal rotPointRadius = 5;
constexpr qreal rotPointMargin = 30;
constexpr qreal maxPointRadius =
	qMax(qMax(sizePointRadius, magPointRadius),
		 qMax(magPointCollideRadius, modiPointRadius));

// 各个图形以及边框的画笔和画刷
const QBrush modiPointBrush(Qt::yellow, Qt::SolidPattern);
const QPen modiPointPen(Qt::black, 1, Qt::SolidLine);

const QBrush sizePointBrush(Qt::white, Qt::SolidPattern);
const QPen sizePointPen(Qt::black, 1, Qt::SolidLine);

const QBrush magPointBrush(Qt::darkGray, Qt::SolidPattern);
const QPen magPointPen(Qt::darkGray, 1, Qt::SolidLine);

const QBrush magPointCollideBursh(Qt::gray, Qt::SolidPattern);
const QPen magPointColidePen(Qt::gray, Qt::SolidLine);

const QBrush rotPointBrush = QBrush(Qt::red, Qt::SolidPattern);
const QPen rotPointPen = QPen(Qt::black, 1, Qt::SolidLine);

const QBrush selectRectBrush = QBrush(Qt::transparent, Qt::SolidPattern);
const QPen selectRectPen = QPen(Qt::black, 1, Qt::DashLine);

// 各个不同图形的注册标识
enum UserTypes
{
	DAbstractBaseType = QGraphicsItem::UserType + 1,
	DShapeBaseType = QGraphicsItem::UserType + 1,
	DLineBaseType = QGraphicsItem::UserType + 2,
	DTextBaseType = QGraphicsItem::UserType + 3,
	DPolygonBaseType = QGraphicsItem::UserType + 4,
	DTextItemType = QGraphicsItem::UserType + 5,
	DRectItemType = QGraphicsItem::UserType + 6,
	DEllItemType = QGraphicsItem::UserType + 7,
	DRoundRectItemType = QGraphicsItem::UserType + 8,
	DLineItemType = QGraphicsItem::UserType + 9,
	DTriItemType = QGraphicsItem::UserType + 10
};

// 常量
namespace DConst
{
	// 圆周率 PI
	constexpr qreal PI = 3.14159265358979323;

	enum NoneType
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
		T = 1,
		B = 2,
		L = 3,
		R = 4,
		TL = 5,
		TR = 6,
		BL = 7,
		BR = 8
	};

	enum LineOrient
	{
		ST = 1,
		ED = 2
	};

	enum LineArrowType
	{
		ARROW = 1,
		OPEN_ARROW = 2,
		DOVETAIL_ARROW = 3,
		DIAMOND_ARROW = 4,
		ROUND_ARROW = 5
	};

	enum Scene
	{
		INSERT_LINE = 1,
		INSERT_SHAPE = 2
	};
};

const int DataStreamVersion = QDataStream::Qt_5_15;
