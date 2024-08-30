#pragma once

#include "global.h"
#include "dshapebase.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>

enum class SceneState
{
	NONE = 0,
	INSERTLINE
};

class DScene : public QGraphicsScene
{
	Q_OBJECT

public:
	DScene();
	DScene(QObject *parent);
	DScene(const QRectF &sceneRect, QObject *parent = nullptr);
	DScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = nullptr);

public:
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

	void addTextItem();
	void addRectItem();
	void addRoundRectItem();
	void addEllItem();
	void addLineItem();
    void addTriItem();

	void delSelectedItem();

	void setMenu(QMenu *m) { menu = m; }

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private:
	static qreal defaultRotateDelta;
	static qreal defaultScaleRatio;
	static qreal defaultMoveDist;

	SceneState state = SceneState::NONE;
	int moditype = DConst::NONE;

	DAbstractBase *modifiedShape = nullptr;
	MagPoint *endMag;
	QMenu *menu = nullptr;
	QPointF endPoint;
};

