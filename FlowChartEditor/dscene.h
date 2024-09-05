#pragma once

#include "global.h"
#include "dallitems.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsSceneEvent>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QMenu>

class DAbstractBase;
class MagPoint;
class DView;

class DScene : public QGraphicsScene
{
	Q_OBJECT

public:
	DScene();
	DScene(QObject *parent);
	DScene(const QRectF &sceneRect, QObject *parent = nullptr);
	DScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = nullptr);

public:
	// 获取被选中图形中的根图形（parent不在列表里），只包含 AbstractBase 下的图形
	QList<DAbstractBase*> getRootSelectedBases();
	// 获取磁吸点与 p 碰撞的图形
	DAbstractBase* getMagItemOnPoint(QPointF p);
	// 获取所有被选中的线条
	QList<DLineBase*> getSelectedLines();
	// 获取所有被选中的图形（包括文本框）
	QList<DShapeBase*> getSelectedShapes();
	QList<DShapeBase*> getNoparentSelectedShapes();

	// 对选中对象的旋转设置
	void resetRotation() { setRotation(0.0); }
	void setRotation(qreal angle = 0.0);
	void rotateSelected(qreal deg = DScene::defaultRotateDelta);
	void rotateCW(qreal deg = DScene::defaultRotateDelta) { rotateSelected(deg); }
	void rotateCCW(qreal deg = DScene::defaultRotateDelta) { rotateSelected(-deg); }

	// 对选中对象的大小设置
	void resetScale() { setScale(1.0); }
	void setScale(qreal scale = 1.0);
	void enlargeSelected(qreal ratio = DScene::defaultScaleRatio);
	void enlarge(qreal ratio = DScene::defaultScaleRatio) { enlargeSelected(ratio); }
	void shrink(qreal ratio = 1 / DScene::defaultScaleRatio) { enlargeSelected(ratio); }

	// 对选中对象的位置设置，
	void resetCenter() { setCenter(0, 0); }
	void setCenter(qreal x = 0, qreal y = 0.0);
	// 以下五个函数为在 view 坐标系下观察，往 view 的某个方向进行移动
	void moveSelected(int distx = 0, int disty = 0);
	void moveLeft(int dist = DScene::defaultMoveDist) { moveSelected(-dist, 0); }
	void moveRight(int dist = DScene::defaultMoveDist) { moveSelected(dist, 0); }
	void moveUp(int dist = DScene::defaultMoveDist) { moveSelected(0, -dist); }
	void moveDown(int dist = DScene::defaultMoveDist) { moveSelected(0, dist); }

	// 调整对象 Z 值
    void moveSelectedZUp();
    void moveSelectedZDown();
	void moveSelectedZ(qreal value = 0.0);
	void moveSelectedZMaxUp();
	void moveSelectedZMaxDown();

	// 插入图形辅助函数
	void prepareInsertItem(DAbstractBase* item);

	// 插入基础图形
	void addRectItem();
	void addRoundRectItem();
	void addEllItem();
	void addTriItem();
	void addDiaItem();
	void addTrapItem();
	void addParagramItem();
    void addpentagonItem();
    void addhexagonItem();

	// 插入流程图图形
	void addDFDocItem();
	void addDFStartEndItem();
	void addDFManualOperateItem();
	void addDFInternalStoreItem();
	void addDFPrepareItem();
	void addDFProcessItem();
	void addDFOptionalProcessItem();
	void addDFConditionItem();
	void addDFDataItem();
	void addDFNodeItem();
	void addManualinItem();
	void addDFPredefineItem();
    void addDFDelayItem();
    void addDFOrItem();
    void addManualInputItem();
	void addDFInformationItem();
    void addDFCardItem();
    void addDFCompareItem();
    void addDFDirecrAccessItem();
    void addDFDiskItem();
    void addDFDisplayItem();
    void addDFMergeItem();
    void addDFMultiDocItem();
    void addDFOffPageItem();
    void addDFPostPoneItem();
    void addDFSequentialAccessItem();
    void addDFStoreDataItem();
    void addDFSummaryconnItem();
    void addDFSortItem();

	// 插入文本框
	void addTextItem();

	// 插入线条
	void addLineItem();
	void addPolyLineItem();
	void addCurveLineItem();

	// 全选、复制、粘贴
	void selectAllItems();
	void copySelectedItems();
	void pasteItems();

	// 删除
	void delSelectedItem();

    // 设置的单个图形选中
    void setItemSelected(QGraphicsItem * item);

    /**
     * @brief ifCollision
     * @param item
     * @return
     * 碰撞检测，线条屏蔽与相同磁吸点连接的线条、与其连接的图形、文本框
     * 图形屏蔽与其连接的线条、文本框
     */
    bool ifCollision(QGraphicsItem * item);

	// 设置画布的菜单、设置绑定在画布上的窗口
	void setMenu(QMenu *m) { menu = m; }
	void setView(DView *v) { view = v; }
	// 获取菜单
	QMenu* getMenu() { return menu; }

	// 清空画布
	void clear();

	void dDrawItems(QList<QGraphicsItem*> items);

	// 调整选中对象的样式
	void changeLineType(Qt::PenStyle linestyle);
	void changeLineWidth(qreal width);
	void changeLineColor(QColor color);

	void changeEndArrow(int endArrowType);
	void changeBeginArrow(int beginArrowType);

	void changeBorderType(Qt::PenStyle linestyle);
	void changeBorderWidth(qreal width);
	void changeBorderColor(QColor color);

	void changeFillType(Qt::BrushStyle brushstyle);
	void changeFillColor(QColor color);
    void changeFillPic(QPixmap pixmap);

    QSet<DTextBase *> getTextBases();
    void changeTextColor(QColor color);
    void changeTextFont(QFont font);

	void changeItemRot(qreal deg = 0.0) { setRotation(deg); }
	void changeItemScale(qreal scl = 1.0) { setScale(scl); }

	void setBackground(QString path);

	// 对齐
	void itemTopAlign();
	void itemBottomAlign();
	void itemLeftAlign();
	void itemRightAlign();
	void itemHorizAlign();
	void itemVertiAlign();

	void itemHorizEven();
	void itemVertiEven();

	// 自动磁吸参考线
	QPointF getAutoAlignItemPos(DShapeBase* item);

	// 设置是否开启磁吸线
	void setAutoAlign(bool active);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private:
	// 初始化画布，设定参数
	void init();
	// 对画布进行快照
	void shot();
    // 获取选中图形的中心点
    QPointF getSelectedItemsCenter() const;
    // 拷贝时的中心点
    QPointF copyPoint;

private:
	// 调整参数
	static qreal defaultRotateDelta; // 旋转时的角度
	static qreal defaultScaleRatio;  // 缩放时的比例
	static int defaultMoveDist;      // 默认移动距离
	static qreal defaultMoveZUp;     // 上移时默认 Z 值
	static qreal defaultMoveZDown;   // 下移时默认 Z 值

	// 绑定的 view 窗口，在本使用情形下，有且仅会有一个窗口
	// 并且不应该为空指针
	DView *view = nullptr;
	// 画布的菜单
	QMenu *menu = nullptr;

	// 插入状态
	// 大体上分为 none, insert, after_insert
	int insert_state = DConst::NONE;

	// 交互状态
	// 分为 none, size, modi, rot
	int inter_state = DConst::NONE;

	// 记录是否在拖拽图形
	// 分为 none, draging
	int drag_state = DConst::NONE;

	// 记录当前画布上显示 magPoint 的图形
	DAbstractBase *showMagedItem = nullptr;

	// 记录当前交互图形的指针
	DAbstractBase *modifiedShape = nullptr;

	// 画布的剪切板
	QByteArray copyData;

	// 用作参考的磁吸线
	QGraphicsLineItem* magLineH = new QGraphicsLineItem(0, 0, 0, 0);
	QGraphicsLineItem* magLineV = new QGraphicsLineItem(0, 0, 0, 0);

	// 设置是否启用自动对齐
	bool autoAlign = true;
};
