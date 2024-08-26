#include "global.h"
#include "dscene.h"
#include "dshapebase.h"
#include "drectitem.h"
#include "dellitem.h"
#include "dlineitem.h"

qreal DScene::defaultRotateDelta = 10;
qreal DScene::defaultScaleRatio = 1.2;
qreal DScene::defaultMoveDist = 50;

DScene::DScene()
{
}

DScene::DScene(QObject *parent)
	: QGraphicsScene(parent)
{
}

DScene::DScene(const QRectF &sceneRect, QObject *parent)
	: QGraphicsScene(sceneRect, parent)
{

}

DScene::DScene(qreal x, qreal y, qreal width, qreal height, QObject *parent)
	: QGraphicsScene(x, y, width, height, parent)
{

}

void DScene::setRotation(qreal angle)
{
	for(QGraphicsItem *item : selectedItems())
		item->setRotation(angle);
}

void DScene::rotateSelected(qreal deg)
{
	for(QGraphicsItem *item : selectedItems())
		item->setRotation(degMod(item->rotation() + deg));
}

void DScene::setScale(qreal scale)
{
	for(QGraphicsItem *item : selectedItems())
		item->setScale(scale);
}

void DScene::enlargeSelected(qreal ratio)
{
	for(QGraphicsItem *item : selectedItems())
		item->setScale(item->scale() * ratio);
}

void DScene::setCenter(qreal x, qreal y)
{
	for(QGraphicsItem *item : selectedItems())
		item->setPos(x, y);
}

void DScene::moveSelected(qreal distx, qreal disty)
{
	for(QGraphicsItem *item : selectedItems())
	{
		QPointF pos = item->pos();
		pos.setX(pos.x() + distx);
		pos.setY(pos.y() + disty);
		item->setPos(pos);
	}
}

void DScene::addTextItem()
{
}

void DScene::addRectItem()
{
	qDebug() << "add rectangle";
	DRectItem *item = new DRectItem(200, 200);
	addItem(item);
}

void DScene::addEllItem()
{
	qDebug() << "add ellipse";
	DEllItem *item = new DEllItem(200, 200);
	addItem(item);
}

void DScene::addLineItem()
{
	qDebug() << "add line";
	state = SceneState::INSERTLINE;
	// DLineItem *item = new DLineItem();
	// item->setLine(-100, -100, 100, 100);
	// item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	// addItem(item);

}

void DScene::delSelectedItem()
{
	qDebug() << "delete selected";
	QList<QGraphicsItem*> items = selectedItems();
	for(QGraphicsItem *item : items)
	{
		this->removeItem(item);
		delete item;
	}
}

void DScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF p = event->scenePos();
	QList<QGraphicsItem *> items = this->items(p);

	if(state == SceneState::INSERTLINE)
	{
		event->accept();
		if(items.empty()) startPoint = p, startItem = nullptr;
		else startItem = dynamic_cast<DShapeBase*>(items.first());
		return;
	}

	if(items.empty())
	{
		QGraphicsScene::mousePressEvent(event);
		return;
	}

	QGraphicsItem *item = items.first();

	if(!item->isSelected())
	{
		QGraphicsScene::mousePressEvent(event);
		return;
	}

	if((modifiedShape = dynamic_cast<DShapeBase*>(item)) != nullptr)
	{
		if(modifiedShape->checkInteractPoint(modifiedShape->mapFromScene(p)))
		{
			modifiedShape->setActiveInteractPoint(modifiedShape->mapFromScene(p));
		}
		else modifiedShape = nullptr;
	}

	QGraphicsScene::mousePressEvent(event);
}

void DScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF p = event->scenePos();

	if(modifiedShape != nullptr)
	{
		event->accept();
		modifiedShape->resizeToPoint(modifiedShape->mapFromScene(p));
		return;
	}
	QGraphicsScene::mouseMoveEvent(event);
}

void DScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	modifiedShape = nullptr;

	if(state==SceneState::INSERTLINE)
	{
		QPointF p = event->scenePos();
		QList<QGraphicsItem *> items = this->items(p);
		DShapeBase *endItem = items.empty() ? nullptr : dynamic_cast<DShapeBase* >(items.first());
		QRectF rc(this->startPoint, p);
		DLineItem *line = new DLineItem();
		line->setLine(rc.width()/2, rc.height()/2, -rc.width()/2, -rc.height()/2);
		line->setPos(rc.center());
		line->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
		line->startShape = endItem;
		line->endShape = startItem;
		if(endItem) endItem->arrows.append(line);
		if(startItem) startItem->arrows.append(line);
		line->updatePosition();
		addItem(line);
		state = SceneState::NONE;
	}
	QGraphicsScene::mouseReleaseEvent(event);
}

void DScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	menu->popup(event->screenPos());
}
