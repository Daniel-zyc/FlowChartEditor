#pragma once

#include "global.h"
#include "dlineitem.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>

class DAbstractBase;
class MagPoint;

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

	void resetRotation() { setRotation(0.0); }
	void setRotation(qreal angle = 0.0);
	void rotateSelected(qreal deg = DScene::defaultRotateDelta);
	void rotateCW(qreal deg = DScene::defaultRotateDelta) { rotateSelected(deg); }
	void rotateCCW(qreal deg = DScene::defaultRotateDelta) { rotateSelected(-deg); }

	void resetScale() { setScale(1.0); }
	void setScale(qreal scale = 1.0);
	void enlargeSelected(qreal ratio = DScene::defaultScaleRatio);
	void enlarge(qreal ratio = DScene::defaultScaleRatio) { enlargeSelected(ratio); }
	void shrink(qreal ratio = 1 / DScene::defaultScaleRatio) { enlargeSelected(ratio); }

	void resetCenter() { setCenter(0, 0); }
	void setCenter(qreal x = 0.0, qreal y = 0.0);
	void moveSelected(qreal distx = 0.0, qreal disty = 0.0);
	void moveLeft(qreal dist = DScene::defaultMoveDist) { moveSelected(-dist, 0); }
	void moveRight(qreal dist = DScene::defaultMoveDist) { moveSelected(dist, 0); }
	void moveUp(qreal dist = DScene::defaultMoveDist) { moveSelected(0, -dist); }
	void moveDown(qreal dist = DScene::defaultMoveDist) { moveSelected(0, dist); }

    void moveSelectedZUp(qreal value = DScene::defaultMoveZUp){moveSelectedZ(value);};
    void moveSelectedZDown(qreal value = DScene::defaultMoveZDown){moveSelectedZ(value);};
    void moveSelectedZ(qreal value = 0.0);

    void moveSelectedZMaxUp();
    void moveSelectedZMaxDown();

	void addTextItem();
	void addRectItem();
	void addRoundRectItem();
	void addEllItem();
	void addLineItem();
    void addTriItem();
    void addDiaItem();
    void addTrapItem();
    void addParallegramItem();
    void addDocItem();
    void addPolyLineItem();
    void addEndItem();
    void addPreItem();

	void combineSelected();
	void seperateSelected();

    void copySelectedItems();
    void pasteItems();

	QList<QGraphicsItem *> getDelete();
	void delSelectedItem();

	DAbstractBase* getMagItemOnPoint(QPointF p);
	DAbstractBase* getInterItemOnPoint(QPointF p);

	void setMenu(QMenu *m) { menu = m; }

    void clear();
    void drawItems(QList<QGraphicsItem*> items);

    QList<DLineBase*> getSelectedLine();
    void changeLineType(Qt::PenStyle linestyle);
    void changeEndArrow(int endArrowType);
    void changeLineWidth(double width);
    void setBg(QString path);

    void check();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private:
	static qreal defaultRotateDelta;
	static qreal defaultScaleRatio;
	static qreal defaultMoveDist;
    static qreal defaultMoveZUp;
    static qreal defaultMoveZDown;

	int state = DConst::NONE;
	int moditype = DConst::NONE;

	DAbstractBase *showMagedItem = nullptr;
	DAbstractBase *modifiedShape = nullptr;

    void shot();

    QByteArray copyData;
};

