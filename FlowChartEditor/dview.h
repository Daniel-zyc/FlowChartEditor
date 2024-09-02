#pragma once

#include "global.h"

#include <QGraphicsView>
#include <QScrollBar>

class DView : public QGraphicsView
{
public:
	DView(QWidget *parent = nullptr);
	DView(QGraphicsScene *scene, QWidget *parent = nullptr);

public:
	void resetRotation() { setRotation(0.0); }
	void setRotation(qreal angle = 0.0);
	void rotateCW(qreal deg = DView::defaultRotateDelta)
	{ setRotation(DTool::degMod(rotation - deg)); }
	void rotateCCW(qreal deg = DView::defaultRotateDelta)
	{ setRotation(DTool::degMod(rotation + deg)); }

	void resetScale() { setScale(1.0); }
	void setScale(qreal scale = 1.0);
	void enlarge(qreal ratio = DView::defaultScaleRatio)
	{
		setScale(scale * ratio);
	}
	void shrink(qreal ratio = 1.0 / DView::defaultScaleRatio)
	{ setScale(scale * ratio); }

	void resetCenter() { setCenter(0, 0); }
	void setCenter(qreal x = 0.0, qreal y = 0.0) { centerOn(x, y); }
	void moveLeft(int pixs = DView::defaultMoveDist)
	{ horizontalScrollBar()->setValue(horizontalScrollBar()->value() - pixs); }
	void moveRight(int pixs = DView::defaultMoveDist)
	{ horizontalScrollBar()->setValue(horizontalScrollBar()->value() + pixs); }
	void moveUp(int pixs = DView::defaultMoveDist)
	{ verticalScrollBar()->setValue(verticalScrollBar()->value() - pixs); }
	void moveDown(int pixs = DView::defaultMoveDist)
	{ verticalScrollBar()->setValue(verticalScrollBar()->value() + pixs); }

	void updateTransMatrix();

protected:
	void init();

protected:
	qreal scale = 1.0, rotation = 0.0;

	static qreal defaultRotateDelta;
	static qreal defaultScaleRatio;
	static qreal defaultMoveDist;
};

