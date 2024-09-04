#pragma once

#include "global.h"
#include "dallitems.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsSceneEvent>
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
	// 获取被选中图形中的根图形，只包含 AbstractBase 下的图形
	QList<DAbstractBase*> getRootSelectedBases();

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
	void moveSelectedZUp(qreal value = DScene::defaultMoveZUp) { moveSelectedZ(value); }
	void moveSelectedZDown(qreal value = DScene::defaultMoveZDown) { moveSelectedZ(-value); }
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
    void addDFAgrreConnectItem();
    void addDFCardItem();
    void addDFCompareItem();
    // void addDFDataItem();
    void addDFDirecrAccessItem();
    void addDFDiskItem();
    void addDFDisplayItem();
    void addDFManulInputItem();
    void addDFMergeItem();
    void addDFMultiDocItem();
    void addDFOffPageItem();
    void addDFOrItem();
    void addDFPostPoneItem();
    void addDFSequentialAccessItem();
    void addDFStoreDataItem();
    void addDFSummaryconnItem();

	// 插入文本框
	void addTextItem();

	// 插入线条
	void addLineItem();
	void addPolyLineItem();
	void addCurveLineItem();

	// 复制、粘贴
	void copySelectedItems();
	void pasteItems();

	// 删除
	void delSelectedItem();

    // 设置的单个图形选中
    void setItemSelected(QGraphicsItem * item);

    // 检测某图形是否碰撞
    bool getCollision(QGraphicsItem * item);

	DAbstractBase* getMagItemOnPoint(QPointF p);

	// 设置画布的菜单、设置绑定在画布上的窗口
	void setMenu(QMenu *m) { menu = m; }
	void setView(DView *v) { view = v; }

	// 清空画布
	void clear();

	QMenu* getMenu() { return menu; }

	void dDrawItems(QList<QGraphicsItem*> items);

	QList<DLineBase*> getSelectedLine();
	void changeLineType(Qt::PenStyle linestyle);
	void changeEndArrow(int endArrowType);
	void changeLineWidth(double width);
	void changeLineColor(QColor color);
	void setBg(QString path);
	void changeItemRot();
	void changeItemScale();

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
	// 分为 size, modi, rot
	int inter_state = DConst::NONE;

	// 记录当前画布上显示 magPoint 的图形
	DAbstractBase *showMagedItem = nullptr;

	// 记录当前交互图形的指针
	DAbstractBase *modifiedShape = nullptr;

	// 画布的剪切板
	QByteArray copyData;
};

