#include "global.h"
#include "dscene.h"
#include "dshapebase.h"
#include "drectitem.h"
#include "droundrectitem.h"
#include "dellitem.h"
#include "dlineitem.h"
#include "dtextitem.h"
#include "dtriitem.h"
#include "ddiaitem.h"
#include "dtrapitem.h"



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
    qDebug() << "add textitem";
    DTextItem *item = new DTextItem(100, 100, "hello world!");
    addItem(item);
}

void DScene::addRectItem()
{
	qDebug() << "add rectangle";
	DRectItem *item = new DRectItem(200, 200);
	item->textItem = new DTextItem(100, 100, "hello world!", item);
	item->textItem->deleteMagPoint();
	addItem(item);
}

void DScene::addRoundRectItem()
{
	qDebug() << "add round rectangle";
	DRoundRectItem *item = new DRoundRectItem(200, 200);
	item->textItem = new DTextItem(100, 100, "hello world!", item);
	item->textItem->deleteMagPoint();
	addItem(item);
}

void DScene::addEllItem()
{
	qDebug() << "add ellipse";
	DEllItem *item = new DEllItem(200, 200);
	item->textItem = new DTextItem(100, 100, "hello world!", item);
	item->textItem->deleteMagPoint();
	addItem(item);
}

void DScene::addLineItem()
{
	qDebug() << "add line";
	state = SceneState::INSERTLINE;
	DLineItem *item = new DLineItem({-100, 0}, {100, 0});
	addItem(item);

}

void DScene::addTriItem()
{
	qDebug() << "add Triangle";
	DTriItem *item = new DTriItem(100, 100);
	// item->textItem = new DTextItem(100, 100, "hello world", item);
	addItem(item);
}

void DScene::addDiaItem()
{
    qDebug() << "add Diamond";
    DDiaItem *item = new DDiaItem(100, 100);
    item->textItem = new DTextItem(50, 50, "hello world", item);
    item->textItem->deleteMagPoint();
    addItem(item);
}

void DScene::addTrapItem()
{
    qDebug() << "add Document";
//    QRectF rect(0, 0, 100, 100); // 你可以根据需要调整矩形的大小和位置
    DTrapItem *item = new DTrapItem(80,100,80);
    item->textItem = new DTextItem(50, 50, "hello world", item);
    item->textItem->deleteMagPoint();
    addItem(item);
}
void DScene::combineSelected()
{
	int cnt=selectedItems().count();
	if (cnt>1)
	{
		QGraphicsItemGroup* group = new QGraphicsItemGroup;  //创建组合
		group->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
		addItem(group);      //添加到场景中

		for (int i=0;i<cnt;i++)     //将选择的图形项添加到组合中
		{
			QGraphicsItem* item=selectedItems().at(0);
			item->setSelected(false);    //取消选择
			item->clearFocus();          //清除焦点状态
			if(item->parentItem() != nullptr) continue;
			group->addToGroup(item);     //添加到组合
		}
	}
}

void DScene::seperateSelected()
{
	int cnt=selectedItems().count();
	if (cnt==1)
	{
		QGraphicsItemGroup  *group = dynamic_cast<QGraphicsItemGroup*>(selectedItems().at(0));
		if(!group) return;
		destroyItemGroup(group); //打散组合
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
	QList<QGraphicsItem *> items = this->items(p);

	// if(state == SceneState::INSERTLINE)
	// {
	// 	event->accept();
	// 	items = this->items(p);
	// 	endPoint = p, endMag = nullptr;
	// 	if(!items.empty())
	// 	{
	// 		DShapeBase *item = dynamic_cast<DShapeBase*>(items.first());
	// 		if(item && item->checkMagPoint(p))
	// 		{
	// 			endMag = item->getMagPoint(p);
	// 		}
	// 	}
	// 	return;
	// }

	// qDebug() << items;

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

	// qDebug() << item;

	if((modifiedShape = dynamic_cast<DAbstractBase*>(item)) != nullptr)
	{
		// qDebug() << "modifiedShape";
		if(modifiedShape->checkInterPoint(p))
		{
			// qDebug() << "modi";
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
	}

	QGraphicsScene::mousePressEvent(event);
}

void DScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	// qDebug() << event->button();
	// if(event->button() != Qt::LeftButton)
	// {

	// 	QGraphicsScene::mouseMoveEvent(event);
	// 	return;
	// }
	QPointF p = event->scenePos();

	// if(state == SceneState::INSERTLINE)
	// {
	// 	event->accept();
	// 	return;
	// }

	if(showMagedItem)
	{
		showMagedItem->setShowMagPoint(false);
		showMagedItem = nullptr;
	}

	QList<QGraphicsItem*> items = this->items(p, Qt::IntersectsItemBoundingRect);

	MagPoint *magPoint = nullptr;
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

	if(moditype != DConst::NONE)
	{
		event->accept();
		// qDebug() << "inter";
		modifiedShape->interToPoint(p, magPoint);
		return;
	}
	// else if(moditype == ModifyType::SIZE)
	// {
	// 	event->accept();
	// 	modifiedShape->resizeToPoint(p);
	// 	return;
	// }

	QGraphicsScene::mouseMoveEvent(event);
}

void DScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	qDebug() << event->button();
	if(event->button() != Qt::LeftButton)
	{
		QGraphicsScene::mouseReleaseEvent(event);
		return;
	}

	moditype = DConst::NONE;
	modifiedShape = nullptr;

	// if(state==SceneState::INSERTLINE)
	// {
	// 	event->accept();
	// 	QPointF p = event->scenePos();
	// 	QList<QGraphicsItem *> items = this->items(p);
	// 	MagPoint *startMag = nullptr;
	// 	if(!items.empty())
	// 	{
	// 		DShapeBase *item = dynamic_cast<DShapeBase*>(items.first());
	// 		if(item && item->checkMagPoint(p))
	// 			startMag = item->getMagPoint(p);
	// 	}
	// 	DLineItem *line = new DLineItem();
	// 	line->setLine(QLineF(p, endPoint));
	// 	line->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	// 	line->startMag = startMag;
	// 	line->endMag = endMag;
	// 	if(endMag) endMag->addLine(line);
	// 	if(startMag) startMag->addLine(line);
	// 	line->updatePosition();
	// 	addItem(line);
	// 	state = SceneState::NONE;
	// 	return;
	// }
	QGraphicsScene::mouseReleaseEvent(event);
}

void DScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	menu->popup(event->screenPos());
}
