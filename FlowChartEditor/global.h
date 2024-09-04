#pragma once

#include <QtCore>
#include <QGraphicsItem>
#include <QBrush>
#include <QPen>

// 全局变量，记录图形是否发生了修改
extern int SHOT_STATE;
// 全局变量，记录同一个图形拷贝次数，用于计算错开距离
extern int PASTE_NUM;

// 序列化时用来判断某图形是否参与序列化
extern QSet<int> registeredTypes;

// 大小、磁吸、磁吸碰撞、调整、旋转点半径

constexpr qreal sizePointRadius = 5;
constexpr qreal magPointRadius = 5;
constexpr qreal magPointCollideRadius = 10;
constexpr qreal modiPointRadius = 5;
constexpr qreal rotPointRadius = 5;
constexpr qreal rotPointMargin = 30;
constexpr qreal maxPenWidth = 30;
constexpr qreal maxPointRadius =
	qMax(qMax(sizePointRadius, magPointRadius),
		 qMax(magPointCollideRadius, modiPointRadius));
constexpr qreal maxBorderRadius = qMax(maxPointRadius, maxPenWidth / 2);
constexpr qreal maxLineRaidus = qMax(maxBorderRadius, maxPenWidth * 5);
constexpr qreal minRectSize = sizePointRadius * 2 + magPointCollideRadius * 2;

// 各个图形以及边框的画笔和画刷
inline const QBrush modiPointBrush(Qt::yellow, Qt::SolidPattern);
inline const QPen modiPointPen(Qt::black, 1, Qt::SolidLine);

inline const QBrush sizePointBrush(Qt::white, Qt::SolidPattern);
inline const QPen sizePointPen(Qt::black, 1, Qt::SolidLine);

inline const QBrush magPointBrush(Qt::darkGray, Qt::SolidPattern);
inline const QPen magPointPen(Qt::darkGray, 1, Qt::SolidLine);

inline const QBrush magPointCollideBursh(Qt::gray, Qt::SolidPattern);
inline const QPen magPointColidePen(Qt::gray, Qt::SolidLine);

inline const QBrush rotPointBrush = QBrush(Qt::red, Qt::SolidPattern);
inline const QPen rotPointPen = QPen(Qt::black, 1, Qt::SolidLine);

inline const QBrush selectRectBrush = QBrush(Qt::NoBrush);
inline const QPen selectRectPen = QPen(Qt::black, 1, Qt::DashLine);

inline const QBrush groupRectBrush = QBrush(Qt::NoBrush);
inline const QPen groupRectPen = QPen(Qt::black, 1, Qt::SolidLine);

// 各个不同图形的注册标识
enum UserTypes
{
	DAbstractBaseType = QGraphicsItem::UserType + 1,
	DShapeBaseType = QGraphicsItem::UserType + 2,
	DLineBaseType = QGraphicsItem::UserType + 3,
	DTextBaseType = QGraphicsItem::UserType + 4,
	DPolygonBaseType = QGraphicsItem::UserType + 5,

	DTextItemType = QGraphicsItem::UserType + 40,

	DRectItemType = QGraphicsItem::UserType + 100,
	DEllItemType = QGraphicsItem::UserType + 101,
	DRoundRectItemType = QGraphicsItem::UserType + 102,
	DTriItemType = QGraphicsItem::UserType + 103,
	DParagramItemType = QGraphicsItem::UserType + 104,
	DTrapItemType = QGraphicsItem::UserType + 105,
	DDiaItemType = QGraphicsItem::UserType + 106,

	DFStartEndItemType = QGraphicsItem::UserType + 200,
	DFPredefineItemType = QGraphicsItem::UserType + 201,
	DFDocumentItemType = QGraphicsItem::UserType + 202,
	DFManualOperateItemType = QGraphicsItem::UserType + 204,
	DFInternalStoreItemType = QGraphicsItem::UserType + 205,
	DFPrepareItemType = QGraphicsItem::UserType + 206,
	DFProcessItemType = QGraphicsItem::UserType + 207,
	DFOptionalProcessItemType = QGraphicsItem::UserType + 208,
	DFConditionItemType = QGraphicsItem::UserType + 209,
	DFDataItemType = QGraphicsItem::UserType + 210,
	DFNodeItemType = QGraphicsItem::UserType + 211,
	DFManualInputItemType = QGraphicsItem::UserType + 212,
	DFInformationItemType = QGraphicsItem::UserType + 213,
    DFDelayItemType = QGraphicsItem::UserType + 214,
    DFOrItemType = QGraphicsItem::UserType + 215,
    DFSummaryconnItemType = QGraphicsItem::UserType + 216,

	DLineItemType = QGraphicsItem::UserType + 300,
	DCurveLineItemType = QGraphicsItem::UserType + 301,
	DPolyLineItemType = QGraphicsItem::UserType + 302
};

enum ErrorType
{
    DFProcessShouldHasMoreThanOneIn = 1,                        // 过程矩形一输入
    DFProcessShouldHasMoreThanOneOut = 2,                        // 过程矩形一输出
    DFProcessNoName = 3,

    DFNodeItemNoName = 400,                 // 连接符号无清晰标注

    DFDataItemShouldOnlyInOrOut = 5,        // 数据应该只一个输入或输出
    DFDataItemNoName,

    DFManualOperateItemShouldHasOneOut = 6, // 手动操作符至少一个输出
    DFManualOperateItemNoName = 700,        // 手动操作未指定

    DFConditionItemShouldHasOneIn = 8,          // 决策符号需要一个输入
    DFConditionItemShouldHasMoreThanTwoOut = 9, // 决策符号至少两个输出
    DFConditionItemNoName = 10,                  // 决策符号未指定

    DFDocumentItemNoName = 110,                 // 文档图形未指定

    // 摘录

    DFPredefineItemShouldHasMoreThanOneIn = 12,          // 预定义至少一个输入
    DFPredefineItemShouldHasMoreThanOneOut = 13,            // 预定义至少一个输出
    DFPredefineItemNoName,

    DFEndItemShouldOnlyInOrOut = 14,                        // 终止开始符号只有一个输入或一个输出

    DFPrepareItemNoName = 15,                           // 准备图形未指定

    DFInternalStoreItemNoName = 16,                          // 内部存储未指定

    ChartFlowNoLinedItem = 17,           // 流程图图形独立无连线
    ChartFlowHasNoTypeArrow = 180,      // 流程图图形存在未指定连线

    NomalItemNoLinedItem = 18,       // 一般图形独立无连线

    RoundLine = 19 ,              // 回环连线

    CollisionItem = 200              // 碰撞图形
};
enum ErrorLevel
{
    WARNING = 1,
    ERROR = 0
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

    enum MagInOrOut
    {
        IN = 1,
        OUT = 2,
        NO_IN_OR_OUT = 3
    };
};

class DAbstractBase;

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

	// 判断是不是 AbstractShape 继承出的元素
	bool isAbstract(int type);

	// 判断是不是 Shape 类型
	bool isShape(int type);

	// 判断是不是流程图用图形
	bool isFlowChartShape(int type);

	// 判断是不是 Line 类型
	bool isLine(int type);

	// 判断是不是 text 类型
	bool isText(int type);

	// p 是否在以 o 为半径 r 为圆心的圆中
	bool inCircle(const QPointF& o, qreal r, const QPointF& p);

	// 将要被拷贝的图形进行一定的平移操作
	void moveItems(const QList<QGraphicsItem*> &items);

	// 过滤掉所有 parent 存在于列表中的元素，以及所有不是 DAbstractBase 的元素
	void filterRootBases(QList<QGraphicsItem*> &items);

    // 获取错误等级
    int getErrorLevel(int ErrorType);
	// 过滤掉所有不是 DAbstractBase 的元素
	void filterBases(QList<QGraphicsItem*> &items);

	QList<DAbstractBase*> itemsToBases(QList<QGraphicsItem*> & items);
};
