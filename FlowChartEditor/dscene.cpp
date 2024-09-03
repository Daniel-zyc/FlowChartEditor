#include "dallitems.h"
#include "dscene.h"
#include "undomanager.h"
#include "serializer.h"

#include <QMessageBox>

qreal DScene::defaultRotateDelta = 10;
qreal DScene::defaultScaleRatio = 1.2;
qreal DScene::defaultMoveDist = 50;
qreal DScene::defaultMoveZUp = 20;
qreal DScene::defaultMoveZDown = -20;

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

void DScene::moveSelectedZ(qreal value){
    for(QGraphicsItem *item : selectedItems()){
        if(item->parentItem() != nullptr) continue;
        qreal z = item->zValue();
        qDebug() << "设置为" << z + value;
        item->setZValue(z + value);
    }
}

void DScene::moveSelectedZMaxUp(){
    QSet<QGraphicsItem*> S;
    QList<QGraphicsItem*> colItems;
    qreal selectedMin = std::numeric_limits<qreal>::max();
    qreal colMax = std::numeric_limits<qreal>::lowest();
    for(QGraphicsItem * item : selectedItems()){
        if(item->parentItem() == nullptr && item->zValue() < selectedMin) selectedMin = item->zValue();
        S.insert(item);
        colItems.append(item->collidingItems());
    }
    for(QGraphicsItem * item : colItems)
        if(!S.contains(item) && item->zValue() > colMax && item->parentItem() == nullptr)
            colMax = item->zValue();
    if(selectedMin > colMax) return;
    qreal dis = colMax - selectedMin + 1;
    for(QGraphicsItem * item : selectedItems())
        if(item->parentItem() == nullptr){
            qreal temdis = item->zValue();
            qDebug() << "将z值设置为" << temdis + dis;
            item->setZValue(temdis + dis);
        }
}

void DScene::moveSelectedZMaxDown(){
    QSet<QGraphicsItem*> S;
    QList<QGraphicsItem*> colItems;
    qreal selectedMax = std::numeric_limits<qreal>::lowest();
    qreal colMin = std::numeric_limits<qreal>::max();
    for(QGraphicsItem * item : selectedItems()){
        if(item->parentItem() == nullptr && item->zValue() > selectedMax) selectedMax = item->zValue();
        S.insert(item);
        colItems.append(item->collidingItems());
    }
    for(QGraphicsItem * item : colItems)
        if(!S.contains(item) && item->zValue() < colMin && item->parentItem() == nullptr){
            colMin = item->zValue();
        }
    if(selectedMax < colMin) return;
    qreal dis = selectedMax - colMin + 1;
    for(QGraphicsItem * item : selectedItems())
        if(item->parentItem() == nullptr){
            qreal temdis = item->zValue();
            qDebug() << "将z值设置为"  << temdis - dis;
            item->setZValue(temdis - dis);
        }
}

void DScene::addTextItem()
{
	qDebug() << "add textitem";

	DTextItem *item = new DTextItem();
	state = DConst::INSERT_TEXT;
	modifiedShape = item;
}

void DScene::addRectItem()
{
	qDebug() << "add rectangle";

	DRectItem *item = new DRectItem();
	state = DConst::INSERT_SHAPE;
	modifiedShape = item;
}

void DScene::addRoundRectItem()
{
	qDebug() << "add round rectangle";
	DRoundRectItem *item = new DRoundRectItem();
	state = DConst::INSERT_SHAPE;
	modifiedShape = item;
}

void DScene::addEllItem()
{
	qDebug() << "add ellipse";
	DEllItem *item = new DEllItem();
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
	state = DConst::INSERT_SHAPE;
	modifiedShape = item;
}

void DScene::addParallegramItem()
{
    qDebug() << "add Parallegram";
	DParallelogramItem *item = new DParallelogramItem();
	state = DConst::INSERT_SHAPE;
	modifiedShape = item;
}

void DScene::addDocItem()
{
	qDebug() << "add Document";
	DDocItem *item = new DDocItem();
	state = DConst::INSERT_SHAPE;
	modifiedShape = item;
}

void DScene::addDiaItem()
{
	qDebug() << "add Diamond";
	DDiaItem *item = new DDiaItem();
	state = DConst::INSERT_SHAPE;
	modifiedShape = item;
}

void DScene::addEndItem()
{
	qDebug() << "add Start/End";
	DEndItem *item = new DEndItem();
	state = DConst::INSERT_SHAPE;
	modifiedShape = item;
}

void DScene::addPreItem()
{
	// qDebug() << "add Document";
	// DEndItem *item = new DEndItem();
	// state = DConst::INSERT_SHAPE;
	// modifiedShape = item;
 //    DTrapItem *item = new DTrapItem(100,80,80);
 //    item->textItem = new DTextItem(50, 50, "", item);
 //    item->textItem->deleteMagPoint();
 //    addItem(item);
}

void DScene::addTrapItem()
{
	qDebug() << "add TrapItem";
	DTrapItem *item = new DTrapItem();
	state = DConst::INSERT_SHAPE;
	modifiedShape = item;
}

void DScene::addPolyLineItem()
{
    qDebug() << "add PolyLine";
    DPolyLineItem *item = new DPolyLineItem();
    state = DConst::INSERT_LINE;
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

	DItemGroup* group = new DItemGroup();  //创建组合
	// group->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	addItem(group);      //添加到场景中

	for(QGraphicsItem* item : items)
	{
		if(item->parentItem() != nullptr) continue;
		group->addToGroup(item);
	}

	// group->setSelected(false);
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
		// for(QGraphicsItem* item : items) item->setSelected(false);
	}
}

QList<QGraphicsItem *> DScene::getDelete()
{
	QList<QGraphicsItem*> items = selectedItems();
	QSet<QGraphicsItem*> S;
	for(QGraphicsItem* item : items) S.insert(item);
	items.clear();
	for(QGraphicsItem *item : S) {
		if(S.contains(item->parentItem())) continue;
		items.push_back(item);
    }
	return items;
}

void DScene::delSelectedItem()
{
	qDebug() << "delete selected";
	QList<QGraphicsItem*> items = getDelete();
	qDebug() << items;
	for(QGraphicsItem *item : items)
	{
        this->removeItem(item);
		DShapeBase* shape; DLineBase* line;
		if((shape = dynamic_cast<DShapeBase*>(item)))
			shape->unLinkAllLines();
		else if((line = dynamic_cast<DLineBase*>(item)))
		{
			line->unlinkBegin();
			line->unlinkEnd();
		}
    }
	for(QGraphicsItem *item : items) delete item;
}

DAbstractBase* DScene::getMagItemOnPoint(QPointF p)
{
	QList<QGraphicsItem*> items = this->items(p, Qt::IntersectsItemBoundingRect);

	for(QGraphicsItem *item : items)
	{
		DAbstractBase *shape = dynamic_cast<DAbstractBase*>(item);
		if(!shape) continue;
		if(shape->checkMagPoint(p))
			return shape;
	}
	return nullptr;
}

DAbstractBase* DScene::getInterItemOnPoint(QPointF p)
{
	return nullptr;
}

void DScene::changeLineType(Qt::PenStyle linestyle)
{
    qDebug() << "change Line Type";
    QList<DLineBase*> lines = getSelectedLine();
    QMessageBox msgBox;
    msgBox.setText("提示");

    for(DLineBase *line : lines) {
        QPen npen = line->pen();
        npen.setStyle(linestyle);
        line->setPen(npen);
    }
}

void DScene::changeEndArrow(int endArrowType)
{
    qDebug() << "change Line endArrow";
    QList<DLineBase*> lines = getSelectedLine();
    QMessageBox msgBox;
    msgBox.setText("提示");

    for(DLineBase *line : lines) {
        line->setEndArrowType(endArrowType);
    }

}

void DScene::changeLineWidth(double width)
{
    qDebug() << "change Line Width";
    QList<DLineBase*> lines = getSelectedLine();
    QMessageBox msgBox;
    msgBox.setText("提示");

    // if(lines.count() < 1) {
    //     msgBox.setInformativeText("请选中线条");
    //     msgBox.exec();
    //     return;
    // }

    for(DLineBase *line : lines) {
        QPen npen = line->pen();
        npen.setWidth(width);
        line->setPen(npen);
    }
}

void DScene::setBg(QString path)
{
    QPixmap bg(path);
    setBackgroundBrush(bg);
}

void DScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)
    {
        QGraphicsScene::mousePressEvent(event);
        return;
    }

	QPointF p = event->scenePos();
	qDebug() << "press pos: " << p;

	if(state == DConst::INSERT_SHAPE || state == DConst::INSERT_LINE
	   || state == DConst::INSERT_TEXT)
	{
		event->accept();
		addItem(modifiedShape);
		if(state == DConst::INSERT_SHAPE || state == DConst::INSERT_TEXT)
		{
			DShapeBase* shape = dynamic_cast<DShapeBase*>(modifiedShape);
			shape->setInsertItem();
			qDebug() << p;
			qDebug() << p + QPointF(shape->sizeRect().width()/2, shape->sizeRect().height()/2);
			shape->setPos(p + QPointF(shape->sizeRect().width()/2, shape->sizeRect().height()/2));
			qDebug() << shape->pos();
		}
		else
		{
			DLineBase* line = dynamic_cast<DLineBase*>(modifiedShape);
			DAbstractBase* shape = getMagItemOnPoint(p);
			if(shape)
				line->linkBeginUpdate(shape->getMagPoint(p));
			else
				line->setBeginPoint(p);
			line->setEndPoint(p);
			line->setInsertItem();
		}
		moditype = DConst::SIZE;
		state = state + 1;
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
		QGraphicsScene::mousePressEvent(event);
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
		DAbstractBase* shape = getMagItemOnPoint(p);
		if(shape)
		{
			magPoint = shape->getMagPoint(p);
			shape->setShowMagPoint(true);
			showMagedItem = shape;
		}
	}

	if(moditype != DConst::NONE)
	{
		event->accept();
		// qDebug() << modifiedShape;
		modifiedShape->interToPoint(p, magPoint);
		if(state == DConst::AFTER_INSERT_SHAPE)
		{
			DShapeBase *shape = dynamic_cast<DShapeBase*>(modifiedShape);
			// qDebug() << shape;
			QRectF rc = shape->sizeRect();
			rc.setRect(rc.left()/2, rc.top()/2, rc.width()/2, rc.height()/2);
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

    if((!selectedItems().empty()
        || state != DConst::NONE)
        && SHOT_STATE == DConst::CHANGED){
        shot();
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

void DScene::shot(){
    UndoManager::instance().shot();
}

void DScene::clear(){
    QGraphicsScene::clear();
	QGraphicsLineItem *line_h = new QGraphicsLineItem(-2000, 0, 2000, 0);
	line_h->setZValue(DConst::LINE_Z_VALUE);
	QGraphicsLineItem *line_v = new QGraphicsLineItem(0, -2000, 0, 2000);
	line_v->setZValue(DConst::LINE_Z_VALUE);
	addItem(line_h);
	addItem(line_v);
    // QGraphicsLineItem *line1 = addLine(-1000, 0, 1000, 0);
    // line1->setZValue(DConst::LINE_Z_VALUE);
    // QGraphicsLineItem *line2 = addLine(0, -1000, 0, 1000);
    // line2->setZValue(DConst::LINE_Z_VALUE);
}

QList<DLineBase *> DScene::getSelectedLine()
{
    QList<QGraphicsItem*> items = selectedItems();
    QList<DLineBase*> lines;

    for(QGraphicsItem *item : items) {
        DLineBase *line = dynamic_cast<DLineBase*>(item);
        if(line != nullptr) {
            lines.push_back(line);
        }
    }

    return lines;
}

void DScene::drawItems(QList<QGraphicsItem*> items){
    for(QGraphicsItem * item : items)
        if(item->parentItem() == nullptr)
            addItem(item);
}

void DScene::copySelectedItems(){
    copyData.clear();
    QDataStream out(&copyData,QIODevice::WriteOnly);
	Serializer::instance().serializeItems(out,this->selectedItems());
}

void DScene::pasteItems(){
    if(copyData.isEmpty()) return;
    QDataStream in(&copyData,QIODevice::ReadOnly);
	QList<QGraphicsItem*> items = Serializer::instance().deserializeItems(in);
    DTool::moveItems(items);
    drawItems(items);
}
