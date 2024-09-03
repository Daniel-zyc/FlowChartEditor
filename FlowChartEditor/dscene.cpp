#include "dallitems.h"
#include "dscene.h"
#include "dview.h"
#include "undomanager.h"
#include "serializer.h"

#include <QMessageBox>

qreal DScene::defaultRotateDelta = 10;
qreal DScene::defaultScaleRatio = 1.1;
int DScene::defaultMoveDist = 5;
qreal DScene::defaultMoveZUp = 20;
qreal DScene::defaultMoveZDown = -20;

DScene::DScene() { init(); }

DScene::DScene(QObject *parent)
	: QGraphicsScene(parent) { init(); }

DScene::DScene(const QRectF &sceneRect, QObject *parent)
	: QGraphicsScene(sceneRect, parent) { init(); }

DScene::DScene(qreal x, qreal y, qreal width, qreal height, QObject *parent)
	: QGraphicsScene(x, y, width, height, parent) { init(); }

void DScene::init()
{
}

QList<DAbstractBase*> DScene::getRootSelectedBases()
{
	QList<QGraphicsItem*> items = selectedItems();
	DTool::filterRootBases(items);
	QList<DAbstractBase*> bases;
	for(QGraphicsItem* item : items) bases.push_back(dynamic_cast<DShapeBase*>(item));
	return bases;
}

void DScene::setRotation(qreal angle)
{
	for(DAbstractBase *item : getRootSelectedBases()) item->setRotation(angle);
}

void DScene::rotateSelected(qreal deg)
{
	for(DAbstractBase *item : getRootSelectedBases())
		item->setRotation(DTool::degMod(item->rotation() + deg));
}

void DScene::setScale(qreal scale)
{
	for(DAbstractBase *item : getRootSelectedBases()) item->setScale(scale);
}

void DScene::enlargeSelected(qreal ratio)
{
	for(DAbstractBase *item : getRootSelectedBases())
		item->setScale(item->scale() * ratio);
}

void DScene::setCenter(qreal x, qreal y)
{
	for(DAbstractBase *item : getRootSelectedBases()) item->setPos(x, y);
}

void DScene::moveSelected(int distx, int disty)
{
	QPointF dir = view->mapToScene(distx, disty) - view->mapToScene(QPoint(0, 0));
	for(DAbstractBase *item : getRootSelectedBases())
	{
		QPointF pos = item->pos();
		pos.setX(pos.x() + dir.x());
		pos.setY(pos.y() + dir.y());
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
        if(!S.contains(item)
            && item->zValue() > colMax
            && item->parentItem() == nullptr
            && dynamic_cast<DAbstractBase*>(item) != nullptr)
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
        if(!S.contains(item)
            && item->zValue() < colMin
            && item->parentItem() == nullptr
            && dynamic_cast<DAbstractBase*>(item) != nullptr){
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


void DScene::prepareInsertItem(DAbstractBase* item)
{
	qDebug() << "prepare insert item";
	qDebug() << "item type: " << item->type();

	if(state == DConst::INSERT_SHAPE || state == DConst::INSERT_SHAPE
	   || state == DConst::INSERT_LINE)
	{
		qDebug() << "delete uninserted item";
		delete modifiedShape;
		modifiedShape = nullptr;
	}

	if(item->isShape()) state = DConst::INSERT_SHAPE;
	if(item->isLine()) state = DConst::INSERT_LINE;
	if(item->isText()) state = DConst::INSERT_LINE;
	modifiedShape = item;
}

//================================= text item ==================================

void DScene::addTextItem()
{
	qDebug() << "add textitem";
	prepareInsertItem(new DTextItem());
}

//================================ basic item ==================================

void DScene::addRectItem()
{
	qDebug() << "add rectangle";
	prepareInsertItem(new DRectItem());
}

void DScene::addRoundRectItem()
{
	qDebug() << "add round rectangle";
	prepareInsertItem(new DRoundRectItem());
}

void DScene::addEllItem()
{
	qDebug() << "add ellipse";
	prepareInsertItem(new DEllItem());
}

void DScene::addTriItem()
{
	qDebug() << "add Triangle";
	prepareInsertItem(new DTriItem());
}

void DScene::addParagramItem()
{
	qDebug() << "add Parallegram";
	prepareInsertItem(new DParagramItem());
}

void DScene::addDiaItem()
{
	qDebug() << "add Diamond";
	prepareInsertItem(new DDiaItem());
}

void DScene::addTrapItem()
{
	qDebug() << "add TrapItem";
	prepareInsertItem(new DTrapItem());
}

//============================ flow chart item =================================

void DScene::addDFDocItem()
{
	qDebug() << "add DFDocItem";
	prepareInsertItem(new DFDocumentItem());
}

void DScene::addDFStartEndItem()
{
	qDebug() << "add DFEndItem";
	prepareInsertItem(new DFStartEndItem());
}

void DScene::addDFManualOperateItem()
{
	qDebug() << "add DFManualOperateItem";
	prepareInsertItem(new DFManualOperateItem());
}

void DScene::addDFInternalStoreItem()
{
	qDebug() << "add DFInternalStoreItem";
	prepareInsertItem(new DFInternalStoreItem());
}
void DScene::addDFPrepareItem()
{
	qDebug() << "add DFPrepareItem";
	prepareInsertItem(new DFPrepareItem());
}
void DScene::addDFDelayItem()
{
    qDebug() << "add DFDelayItem";
    prepareInsertItem(new DFDelayItem());
}

void DScene::addDFProcessItem()
{
	qDebug() << "add DFProcessItem";
	prepareInsertItem(new DFProcessItem());
}

void DScene::addDFOptionalProcessItem()
{
	qDebug() << "add DFOptionalProcessItem";
	prepareInsertItem(new DFOptionalProcessItem());
}

void DScene::addDFConditionItem()
{
	qDebug() << "add DFConditionItem";
	prepareInsertItem(new DFConditionItem());
}

void DScene::addDFDataItem()
{
	qDebug() << "add DFDataItem";
	prepareInsertItem(new DFDataItem());
}

void DScene::addDFNodeItem()
{
	qDebug() << "add DFNodeItem";
	prepareInsertItem(new DFNodeItem());
}

void DScene::addDFPredefineItem()
{
	qDebug() << "add DFNodeItem";
	prepareInsertItem(new DFPredefineItem());
}

//=============================== line item ====================================

void DScene::addLineItem()
{
	qDebug() << "add line";
	prepareInsertItem(new DLineItem());
}
void DScene::addManualinItem()
{
    qDebug() << "add ManualinItem";
    DFManualinputItem *item = new DFManualinputItem();
    state = DConst::INSERT_SHAPE;
    modifiedShape = item;
}

void DScene::addPolyLineItem()
{
	qDebug() << "add PolyLine";
	prepareInsertItem(new DPolyLineItem());
}

void DScene::addCurveLineItem()
{
	qDebug() << "add CurveLine";
	prepareInsertItem(new DCurveLineItem());
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

void DScene::changeLineColor(QColor color)
{
    qDebug() << "change Line Color";
    QList<DLineBase*> lines = getSelectedLine();

    for(DLineBase *line : lines) {
        QPen npen = line->pen();
        npen.setColor(color);
        line->setPen(npen);
    }
}

void DScene::setBg(QString path)
{
    QPixmap bg(path);
    setBackgroundBrush(bg);
}

void DScene::changeItemRot()
{

}

void DScene::changeItemScale()
{

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
		modifiedShape->setSelected(true);
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

void DScene::clear()
{
	QGraphicsScene::clear();

	QGraphicsLineItem *line_h = new QGraphicsLineItem(-2000, 0, 2000, 0);
	QGraphicsLineItem *line_v = new QGraphicsLineItem(0, -2000, 0, 2000);
	addItem(line_h);
	addItem(line_v);
	line_h->setVisible(false);
	line_v->setVisible(false);
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

void DScene::dDrawItems(QList<QGraphicsItem*> items){
    for(QGraphicsItem * item : items)
        if(item->parentItem() == nullptr)
            addItem(item);
}

void DScene::copySelectedItems(){
    PASTE_NUM = 1;
    copyData.clear();
    QDataStream out(&copyData,QIODevice::WriteOnly);
	Serializer::instance().serializeItems(out,this->selectedItems());
}

void DScene::pasteItems(){
    if(copyData.isEmpty()) return;
    QDataStream in(&copyData,QIODevice::ReadOnly);
	QList<QGraphicsItem*> items = Serializer::instance().deserializeItems(in);
    DTool::moveItems(items);
    dDrawItems(items);
}
