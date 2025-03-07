#include "dview.h"

qreal DView::defaultRotateDelta = 10;
qreal DView::defaultScaleRatio = 1.2;
qreal DView::defaultMoveDist = 50;

DView::DView(QWidget *parent)
	: QGraphicsView(parent)
{ init(); }

DView::DView(QGraphicsScene *scene, QWidget *parent)
	: QGraphicsView(scene, parent)
{ init(); }

void DView::init()
{
	setMouseTracking(true);
	setRenderHint(QPainter::Antialiasing, true);
	setResizeAnchor(QGraphicsView::AnchorViewCenter);
	setDragMode(QGraphicsView::RubberBandDrag);
	setScale(scale);
}

void DView::setRotation(qreal angle)
{
	rotation = angle;
	updateTransMatrix();
}

void DView::setScale(qreal scale)
{
	this->scale = scale;
	updateTransMatrix();
}

void DView::updateTransMatrix()
{
	QTransform matrix;
	matrix.scale(scale, scale);
	matrix.rotate(rotation);
	setTransform(matrix);
	update();
}

void DView::mouseMoveEvent(QMouseEvent* event)
{
	QPointF p = event->pos();
	viewLabel->setText(QString::asprintf(
						   "%.1lf, %.1lf", p.x()/globalScale, p.y()/globalScale));
	p = mapToScene({(int)p.x(), (int)p.y()});
	sceneLabel->setText(QString::asprintf(
							"%.1lf, %.1lf", p.x()/globalScale, p.y()/globalScale));
	QGraphicsView::mouseMoveEvent(event);
}
