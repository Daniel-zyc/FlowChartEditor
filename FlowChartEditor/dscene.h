#pragma once

#include "global.h"
#include "dallitems.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
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
	QMenu *menu = nullptr;

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
	QList<QGraphicsItem *> getDelete();
	void delSelectedItem();

	DAbstractBase* getMagItemOnPoint(QPointF p);
	DAbstractBase* getInterItemOnPoint(QPointF p);

	void setMenu(QMenu *m) { menu = m; }

	void clear();
	void dDrawItems(QList<QGraphicsItem*> items);

	QList<DLineBase*> getSelectedLine();
	void changeLineType(Qt::PenStyle linestyle);
	void changeEndArrow(int endArrowType);
	void changeLineWidth(double width);
	void changeLineColor(QColor color);
	void setBg(QString path);
    void changeItemRot();
    void changeItemScale();


	void setView(DView *v) { view = v; }

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private:
	void init();
	void shot();

private:
	static qreal defaultRotateDelta;
	static qreal defaultScaleRatio;
	static int defaultMoveDist;
	static qreal defaultMoveZUp;
	static qreal defaultMoveZDown;

	// 绑定的 view 窗口，在本使用情形下，有且仅会有一个窗口
	DView *view;

	int state = DConst::NONE;
	int moditype = DConst::NONE;

	DAbstractBase *showMagedItem = nullptr;
	DAbstractBase *modifiedShape = nullptr;

	QByteArray copyData;
};

