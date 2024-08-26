#include "dview.h"

qreal DView::defaultRotateDelta = 10;
qreal DView::defaultScaleRatio = 1.2;
qreal DView::defaultMoveDist = 50;

DView::DView(QWidget *parent)
	: QGraphicsView(parent), scale(1.0), rotation(0.0)
{
	setRenderHint(QPainter::Antialiasing, true);
}

DView::DView(QGraphicsScene *scene, QWidget *parent)
	: QGraphicsView(scene, parent), scale(1.0), rotation(0.0)
{

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
