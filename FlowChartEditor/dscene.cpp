#include "global.h"
#include "dscene.h"
#include "dshapebase.h"
#include "drectitem.h"
#include "droundrectitem.h"
#include "dellitem.h"
#include "dlineitem.h"
#include "dtextitem.h"
#include "dtriitem.h"
#include "ditemgroup.h"

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
		item->setRotation(DTool::degMod(item->rotation() + deg));
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
	qDebug() << "add textitem";
	DTextItem *item = new DTextItem();
	state = DConst::INSERT_SHAPE;
	modifiedShape = item;
}

void DScene::addRectItem()
{
	qDebug() << "add rectangle";

	DRectItem *item = new DRectItem();
	item->textItem = new DTextItem("", item);
	state = DConst::INSERT_SHAPE;
	modifiedShape = item;
}

void DScene::addRoundRectItem()
{
	qDebug() << "add round rectangle";
	DRoundRectItem *item = new DRoundRectItem();
	item->textItem = new DTextItem("", item);
	state = DConst::INSERT_SHAPE;
	modifiedShape = item;
}

void DScene::addEllItem()
{
	qDebug() << "add ellipse";
	DEllItem *item = new DEllItem();
	item->textItem = new DTextItem("", item);
	state = DConst::INSERT_SHAPE;
	modifiedShape = item;
}

void DScene::addLineItem()
{
	qDebug() << "add line";
	DLineItem *item = new DLineItem();
	state = DConst::INSERT_LINE;
	modifiedShape = item;
}

void DScene::addTriItem()
{
	qDebug() << "add Triangle";
	DTriItem *item = new DTriItem();
	item->textItem = new DTextItem("", item);
	state = DConst::INSERT_SHAPE;
	modifiedShape = item;
}

void DScene::combineSelected()
{
    QList<QGraphicsItem*> items = selectedItems();
    int cnt = 0;
    for(QGraphicsItem* item : items)
    {
        if(item->parentItem() != nullptr) continue;
        cnt++;
    }
	if(cnt <= 1) return;

	DItemGroup* group = new DItemGroup;  //创建组合
    group->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    addItem(group);      //添加到场景中

    for(QGraphicsItem* item : items)
    {
        if(item->parentItem() != nullptr) continue;
        group->addToGroup(item);
    }

    group->setSelected(false);
}

void DScene::seperateSelected()
{
	int cnt=selectedItems().count();
	if (cnt==1)
	{
		DItemGroup *group = dynamic_cast<DItemGroup*>(selectedItems().at(0));
		if(!group) return;
		destroyItemGroup(group);
		QList<QGraphicsItem*> items = this->items();
		for(QGraphicsItem* item : items) item->setSelected(false);
	}
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
    qDebug() << event->button();
    if(event->button() != Qt::LeftButton)
    {
        QGraphicsScene::mousePressEvent(event);
        return;
    }

	QPointF p = event->scenePos();

	if(state == DConst::INSERT_SHAPE || state == DConst::INSERT_LINE)
	{
		event->accept();
		addItem(modifiedShape);
		modifiedShape->setInsertItem();
		modifiedShape->setPos(p);
		state = state + 2;
		moditype = DConst::SIZE;
		return;
	}

	QList<QGraphicsItem *> items = this->items(p);

	if(items.empty())
	{
		QGraphicsScene::mousePressEvent(event);
		return;
	}

    QGraphicsItem *item = items.first();

    if(!item->isSelected())
    {
        QGraphicsScene::mousePressEvent(event); // ??
        return;
    }

	if((modifiedShape = dynamic_cast<DAbstractBase*>(item)) != nullptr)
	{
		if(modifiedShape->checkInterPoint(p))
		{
			moditype = modifiedShape->setInterPoint(p);
		}
		else moditype = DConst::NONE;
	}

        // qDebug() << "modifiedShape";
        if(modifiedShape->checkInterPoint(p))
        {
            // qDebug() << "modi";
            // qDebug() << modifiedShape->checkInterPoint(p);
            moditype = modifiedShape->setInterPoint(p);
        }
        else moditype = DConst::NONE;
        // else if(modifiedShape->checkSizePoint(p))
        // {
        // 	modifiedShape->setSizePoint(p);
        // 	moditype = ModifyType::SIZE;
        // }
        // else
        // {
        // 	modifiedShape = nullptr;
        // 	moditype = ModifyType::NONE;
        // }

    QGraphicsScene::mousePressEvent(event);
}


void DScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF p = event->scenePos();

	if(showMagedItem)
	{
		showMagedItem->setShowMagPoint(false);
		showMagedItem = nullptr;
	}

	MagPoint *magPoint = nullptr;
	if(dynamic_cast<DLineBase*>(modifiedShape))
	{
		QList<QGraphicsItem*> items = this->items(p, Qt::IntersectsItemBoundingRect);

		for(QGraphicsItem *item : items)
		{
			DAbstractBase *shape = dynamic_cast<DAbstractBase*>(item);
			if(!shape) continue;
			if(shape->checkMagPoint(p))
			{
				shape->setShowMagPoint(true);
				showMagedItem = shape;
				magPoint = shape->getMagPoint(p);
				break;
			}
		}
	}

	if(moditype != DConst::NONE)
	{
		event->accept();
		modifiedShape->interToPoint(p, magPoint);
		// qDebug() << moditype;
		if(state == DConst::AFTER_INSERT_SHAPE)
		{
			DShapeBase *shape = dynamic_cast<DShapeBase*>(modifiedShape);
			QRectF rc = shape->sizeRect();
			// qDebug() << rc;
			rc.setRect(rc.left()/2, rc.top()/2, rc.width()/2, rc.height()/2);
			// qDebug() << rc;
			shape->textItem->sizeToRect(rc);
		}
		return;
	}

	QGraphicsScene::mouseMoveEvent(event);
}

void DScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

	if(event->button() != Qt::LeftButton)
	{
		QGraphicsScene::mouseReleaseEvent(event);
		return;
	}

	state = DConst::NONE;
	moditype = DConst::NONE;
	modifiedShape = nullptr;

	QGraphicsScene::mouseReleaseEvent(event);
}

void DScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	menu->popup(event->screenPos());
}
