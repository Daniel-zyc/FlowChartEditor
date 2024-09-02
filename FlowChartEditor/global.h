#pragma once

#include <QtCore>
#include <QGraphicsItem>
#include <QBrush>
#include <QPen>

// 全局变量，记录图形是否发生了修改
extern int SHOT_STATE;

// 序列化时用来判断某图形是否参与序列化
extern QSet<int> registeredTypes;

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
constexpr qreal minRectSize = sizePointRadius * 2 + magPointCollideRadius * 2;

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

const QBrush selectRectBrush = QBrush(Qt::NoBrush);
const QPen selectRectPen = QPen(Qt::black, 1, Qt::DashLine);

const QBrush groupRectBrush = QBrush(Qt::NoBrush);
const QPen groupRectPen = QPen(Qt::black, 1, Qt::SolidLine);

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
	DTriItemType = QGraphicsItem::UserType + 10,
	DParallegramItemType = QGraphicsItem::UserType + 11,
	DDocItemType = QGraphicsItem::UserType + 12,
	DDiaItemType = QGraphicsItem::UserType + 13,
	DTrapItemType = QGraphicsItem::UserType + 14,
    DItemGroupType = QGraphicsItem::UserType + 15,
    DEndItemType = QGraphicsItem::UserType + 16,
    DPreItemType = QGraphicsItem::UserType + 17
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
		AFTER_INSERT_LINE = 2,
		INSERT_SHAPE = 3,
		AFTER_INSERT_SHAPE = 4,
		INSERT_TEXT = 5,
		AFTER_INSERT_TEXT = 6
    };

    enum PasteShift
    {
		SHIFT_X = 30,
		SHIFT_Y = 30
    };

    enum DataStreamVersion
    {
        DATA_STREAM_VERSION = QDataStream::Qt_6_7
    };

    enum RedoAndUndoStackSize
    {
        MAX_UNDO_STACK_SIZE = 25,
        MAX_REDO_STACK_SIZE = 25
    };

    enum ITEM_CHANGE
    {
        CHANGED = 0,
        UNCHANGED = 1
    };

    enum LINE_Z
    {
        LINE_Z_VALUE = -100
    };
};

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

	// p 是否在以 o 为半径 r 为圆心的圆中
	bool inCircle(const QPointF& o, qreal r, const QPointF& p);

	// 将要被拷贝的图形进行一定的平移操作
	void moveItems(const QList<QGraphicsItem*> &items);

	// 过滤掉所有 parent 存在于列表中的元素
	void filterRootItem(QList<QGraphicsItem*> &items);
};
