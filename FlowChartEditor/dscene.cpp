#include "dallitems.h"
#include "dscene.h"
#include "dview.h"
#include "undomanager.h"
#include "serializer.h"

#include <QMessageBox>
#include <inspector.h>

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
	clear();
	magLineH->setPen(magLinePen);
	magLineV->setPen(magLinePen);
}

void DScene::shot()
{
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

QList<DAbstractBase*> DScene::getRootSelectedBases()
{
	QList<QGraphicsItem*> items = selectedItems();
	DTool::filterRootBases(items);
	return DTool::itemsToBases(items);
}

QList<DLineBase *> DScene::getSelectedLines()
{
	QList<DAbstractBase*> items = DTool::itemsToBases(selectedItems());
	QList<DLineBase*> lines;
	for(DAbstractBase *item : items)
		if(item->isLine())
			lines.push_back(dynamic_cast<DLineBase*>(item));
	return lines;
}

QList<DShapeBase*> DScene::getSelectedShapes()
{
	QList<DAbstractBase*> items = DTool::itemsToBases(selectedItems());
	QList<DShapeBase*> shapes;
	for(DAbstractBase* item : items)
		if(item->isShape() || item->isText())
			shapes.push_back(dynamic_cast<DShapeBase*>(item));
	return shapes;
}

QList<DShapeBase*> DScene::getNoparentSelectedShapes()
{
	QList<DAbstractBase*> items = DTool::itemsToBases(selectedItems());
	QList<DShapeBase*> shapes;
	for(DAbstractBase* item : items)
		if(item->parentItem() == nullptr &&
		   (item->isShape() || item->isText()))
			shapes.push_back(dynamic_cast<DShapeBase*>(item));
	return shapes;
}

DAbstractBase* DScene::getMagItemOnPoint(QPointF p)
{
	QList<QGraphicsItem*> items = this->items(p, Qt::IntersectsItemBoundingRect);
	QList<DAbstractBase*> bases = DTool::itemsToBases(items);
	for(DAbstractBase *item : bases)
	{
		if(item->checkMagPoint(p))
			return item;
		if(item->isShape() && item->contains(p))
			return nullptr;
		if(item->isText() && item->parentItem() == nullptr && item->contains(p))
			return nullptr;
	}
	return nullptr;
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
        item->setZValue(z + value);
    }
}

void DScene::moveSelectedZUp(){
    int minColZ = std::numeric_limits<qreal>::max();
    int minSelected = std::numeric_limits<qreal>::max();
    int maxSelected = std::numeric_limits<qreal>::lowest();
    QGraphicsItem * minColItem = nullptr;
    QList<QGraphicsItem*> colItems;
    QSet<QGraphicsItem *> S;
    for(QGraphicsItem *item : selectedItems()){     // 找到选择的最小的z
        if(item->parentItem() != nullptr) continue;
        S.insert(item);
        colItems.append(item->collidingItems());
        if(minSelected > item->zValue()) minSelected = item->zValue();
        if(maxSelected < item->zValue()) maxSelected = item->zValue();
    }
    for(QGraphicsItem *item : colItems){            // 找到最小的碰撞z
        if(item->parentItem() != nullptr || S.contains(item)) continue;
        if(minColZ > item->zValue() && item->zValue() > maxSelected){
            minColItem = item;
            minColZ = item->zValue();
        }
    }
    if(minColItem == nullptr) minColZ = TOTAL_MAX_Z_VALUE + 1;
    int dis = minColZ - minSelected + 1;            // 将选择的所有插入到最小的选择z上面
    for(QGraphicsItem * item : selectedItems()){
        if(item->parentItem() != nullptr) continue;
        qreal temdis = item->zValue() + dis;
        item->setZValue(temdis);
    }

    for(QGraphicsItem * item : this->items()){
        if(item->parentItem() != nullptr || S.contains(item)) continue;
        if(item->zValue() > minColZ){
            qreal temdis = item->zValue() + dis + 2;
            item->setZValue(temdis);
            if(temdis > TOTAL_MAX_Z_VALUE) TOTAL_MAX_Z_VALUE = temdis + 1;
        }
    }
    normalizeZValues();
}
void DScene::moveSelectedZDown(){
    int maxSelected = std::numeric_limits<qreal>::lowest();
    int minSelected = std::numeric_limits<qreal>::max();
    int maxCol = std::numeric_limits<qreal>::lowest();
    QSet<QGraphicsItem *> S;
    QList<QGraphicsItem *> col;
    QGraphicsItem * maxColItem = nullptr;

    for(QGraphicsItem *item : selectedItems()){     // 找到选择的最大值和最小值
        if(item->parentItem() != nullptr) continue;
        col.append(item->collidingItems());
        S.insert(item);
        if(maxSelected < item->zValue()) maxSelected = item->zValue();
        if(minSelected > item->zValue()) minSelected = item->zValue();
    }

    for(QGraphicsItem * item : col){                // 找到碰撞的在选择的之下的最大值
        if(item->parentItem() != nullptr || S.contains(item)) continue;
        if(item->zValue() > maxCol && item->zValue() < minSelected){
            maxColItem = item;
            maxCol = item->zValue();
        }
    }
    int dis = maxSelected - maxCol + 1;
    if(maxColItem != nullptr){                      // 上移动最大碰撞item
        qDebug() << dis+maxColItem->zValue();
        qreal tem = dis + maxColItem->zValue();
        maxColItem->setZValue(tem);
    }else{
        maxCol = -1;
        dis = maxSelected - maxCol + 1;
    }
    for(QGraphicsItem * item : this->items()){      // 将所有大于最大z的全部上移
        if(item->parentItem() != nullptr || S.contains(item) || item == maxColItem) continue;
        if(item->zValue() > maxSelected){
            qreal temdis = item->zValue() + dis + 2;
            item->setZValue(temdis);
            if(temdis > TOTAL_MAX_Z_VALUE) TOTAL_MAX_Z_VALUE = temdis + 1;
        }
    }
    normalizeZValues();
}

void DScene::moveSelectedZMaxDown(){
    QList<QGraphicsItem *>items = this->items();
    QSet<QGraphicsItem *>S;
    qreal maxSelected = std::numeric_limits<qreal>::lowest();

    for(QGraphicsItem * item : selectedItems()){
        if(item->parentItem() != nullptr) continue;
        S.insert(item);
        if(item->zValue() > maxSelected) maxSelected = item->zValue();
    }

    for(QGraphicsItem *item : items){
        if(item->parentItem() != nullptr || S.contains(item)) continue;
        qreal temZ = item->zValue() + maxSelected + 1;
        if(temZ > TOTAL_MAX_Z_VALUE) TOTAL_MAX_Z_VALUE = temZ +1;
        item->setZValue(temZ);
    }
    normalizeZValues();
}

void DScene::moveSelectedZMaxUp(){
    QList<QGraphicsItem*> items = selectedItems();
    for(QGraphicsItem * item : items){
        if(item->parentItem() != nullptr) continue;
        item->setZValue(TOTAL_MAX_Z_VALUE++);
    }
    normalizeZValues();
}

void DScene::normalizeZValues() {
    QList<QGraphicsItem *> itemsList = this->items();
    DTool::filterRootBases(itemsList);

    std::sort(itemsList.begin(), itemsList.end(), [](QGraphicsItem *a, QGraphicsItem *b) {
        return a->zValue() < b->zValue();
    });

    qreal newZValue = 0;
    for (QGraphicsItem *item : itemsList) {
        item->setZValue(newZValue);
        qDebug() << item->zValue();
        ++newZValue;
    }
    TOTAL_MAX_Z_VALUE = newZValue;
}


void DScene::prepareInsertItem(DAbstractBase* item)
{
    // qDebug() << "prepare insert item";
    // qDebug() << "item type: " << item->type();

	if(insert_state == DConst::INSERT_SHAPE || insert_state == DConst::INSERT_TEXT
	   || insert_state == DConst::INSERT_LINE)
	{
        // qDebug() << "delete uninserted item";
		delete modifiedShape;
		modifiedShape = nullptr;
	}

	if(item->isShape()) insert_state = DConst::INSERT_SHAPE;
	if(item->isLine()) insert_state = DConst::INSERT_LINE;
	if(item->isText()) insert_state = DConst::INSERT_TEXT;
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

void DScene::addpentagonItem()
{
    qDebug() << "add 五边形";
    prepareInsertItem(new pentagonItem());
}

void DScene::addhexagonItem()
{
    qDebug() << "add 六边形";
    prepareInsertItem(new DFPrepareItem());
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

void DScene::addDFInformationItem()
{
	qDebug() << "add DFInformationItem";
	prepareInsertItem(new DFInformationItem());
}

void DScene::addDFCardItem()
{
    qDebug() << "add 卡片";
    prepareInsertItem(new DFCardItem());
}

void DScene::addDFCompareItem()
{
    qDebug() << "add 对照";
    prepareInsertItem(new DFCompareItem());
}

void DScene::addDFDirecrAccessItem()
{
    qDebug() << "add 直接访问存储器";
    prepareInsertItem(new DFDirectStorageItem());
}

void DScene::addDFDiskItem()
{
    qDebug() << "add 磁盘";
    prepareInsertItem(new DFDiskItem());
}

void DScene::addDFDisplayItem()
{
    qDebug() << "add 显示";
    prepareInsertItem(new DFShowItem());
}

void DScene::addDFMergeItem()
{
    qDebug() << "add 合并";
    prepareInsertItem(new DFMergeItem());
}

void DScene::addDFMultiDocItem()
{
    qDebug() << "add 多文档";
    prepareInsertItem(new DFMultiDocItem());
}

void DScene::addDFOffPageItem()
{
    qDebug() << "add 离页连接符";
    prepareInsertItem(new DFOffPageItem());
}


void DScene::addDFSequentialAccessItem()
{
    qDebug() << "add 顺序访问存储器";
    prepareInsertItem(new DFOrderStorageItem());
}

void DScene::addDFStoreDataItem()
{
    qDebug() << "add 存储数据";
    prepareInsertItem(new DFStoreDataItem());
}

void DScene::addDFProcessItem()
{
	qDebug() << "add DFProcessItem";
	prepareInsertItem(new DFProcessItem());
}

void DScene::addDFOrItem()
{
    qDebug() << "add DFOrItem";
    prepareInsertItem(new DFOrItem());
}

void DScene::addDFSummaryconnItem()
{
    qDebug() << "add DFSummaryconnItem";
    prepareInsertItem(new DFSummaryconnItem());
}

void DScene::addDFSortItem()
{
    qDebug() << "add 排序";
    prepareInsertItem(new DFSortItem());
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
	qDebug() << "add DFPredefineItem";
	prepareInsertItem(new DFPredefineItem());
}

void DScene::addManualInputItem()
{
	qDebug() << "add DFManualInput";
	prepareInsertItem(new DFManualInputItem());
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
	DFManualInputItem *item = new DFManualInputItem();
	insert_state = DConst::INSERT_SHAPE;
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

void DScene::delSelectedItem()
{
	qDebug() << "delete selected items";
	QList<DAbstractBase*> items = getRootSelectedBases();
	qDebug() << "deleteed items: " << items;
	for(DAbstractBase *item : items)
	{
		DShapeBase* shape; DLineBase* line;
		if((shape = dynamic_cast<DShapeBase*>(item))) shape->unLinkAllLines();
		else if((line = dynamic_cast<DLineBase*>(item)))
		{
			line->unlinkBeginUpdate();
			line->unlinkEndUpdate();
		}
		if(item->isText() && item->parentItem())
			dynamic_cast<DShapeBase*>(item->parentItem())->textItem = nullptr;
		item->setParentItem(nullptr);
	}
	for(DAbstractBase* item : items)
	{
		removeItem(item);
		delete item;
	}
}

void DScene::setItemSelected(QGraphicsItem * item){
    for(QGraphicsItem * selectedItem : selectedItems())
        selectedItem -> setSelected(false);
    item->setSelected(true);
}

bool DScene::ifCollision(QGraphicsItem * item){
    QList<QGraphicsItem * >items = item->collidingItems();
    if(items.empty()) return false;
    DTool::filterRootBases(items);      // 过滤子item和非abstractBase的item
    DAbstractBase * base = dynamic_cast<DAbstractBase *>(item);
    // 线条碰撞的检测，应该屏蔽与同一个磁吸点连线的和所连接的图形以及文本框
    if(DTool::isLine(base->type())){
        DLineBase *lineBase = dynamic_cast<DLineBase*>(base);
        if(lineBase == nullptr) return false;
        for (int i = items.size() - 1; i >= 0; i--) {
            DAbstractBase *abstractBase = dynamic_cast<DAbstractBase*>(items[i]);

            // 去除与其有连线的图形
            if (abstractBase != nullptr && lineBase->ifLinkedWith(abstractBase)) {
                qDebug() << "去除有连线";
                items.removeAt(i);
                continue;
            }

            // 去除连接相同磁吸点的线
            DLineBase *otherLineBase = dynamic_cast<DLineBase*>(items[i]);
            if (otherLineBase != nullptr && lineBase->ifLinedSameMag(otherLineBase)) {
                qDebug() << "去除有相同磁吸点";
                items.removeAt(i);
                continue;
            }

            // 去除文本框的碰撞
            DTextItem *textItem = dynamic_cast<DTextItem*>(items[i]);
            if (textItem) {
                items.removeAt(i);
                continue;
            }
        }
        if(items.empty()) return false;
        return true;
    }

    if(DTool::isShape(base->type())){
        qDebug() << "开始检测图形碰撞";
        qDebug() << items.size();
        DShapeBase *shapeBase = dynamic_cast<DShapeBase*>(base);
        if(shapeBase == nullptr) return false;
        for (int i = items.size() - 1; i >= 0; i--) {
            DTextItem *otherTextItem = dynamic_cast<DTextItem*>(items[i]);
            if (otherTextItem) {
                items.removeAt(i);
                continue;
            }
            DLineBase *otherlineBase = dynamic_cast<DLineBase*>(items[i]);
            if (otherlineBase != nullptr && otherlineBase->ifLinkedWith(shapeBase)) {
                items.removeAt(i);
                continue;
            }
        }
        if(items.empty()) return false;
        return true;
    }

    if(!items.empty()) return true;
    return false;
}

//==============================================================================

void DScene::changeLineType(Qt::PenStyle linestyle)
{
	qDebug() << "change Line Type";
	for(DLineBase *line : getSelectedLines()) {
		QPen npen = line->pen();
		npen.setStyle(linestyle);
		line->setPen(npen);
	}
}

void DScene::changeLineWidth(qreal width)
{
	qDebug() << "change Line Width";
	for(DLineBase *line : getSelectedLines()) {
		QPen npen = line->pen();
		npen.setWidthF(width);
		line->setPen(npen);
	}
}

void DScene::changeLineColor(QColor color)
{
	qDebug() << "change Line Color";
	for(DLineBase *line : getSelectedLines()) {
		QPen npen = line->pen();
		npen.setColor(color);
		line->setPen(npen);
	}
}

void DScene::changeBeginArrow(int beginArrowType)
{
	qDebug() << "change Line beginArrow";
	for(DLineBase *line : getSelectedLines())
		line->setBeginArrowType(beginArrowType);
}

void DScene::changeEndArrow(int endArrowType)
{
	qDebug() << "change Line endArrow";
	for(DLineBase *line : getSelectedLines())
		line->setEndArrowType(endArrowType);
}

void DScene::changeBorderType(Qt::PenStyle linestyle)
{
	qDebug() << "change border type";
	for(DShapeBase* shape : getSelectedShapes())
	{
		QPen npen = shape->pen();
		npen.setStyle(linestyle);
		shape->setPen(npen);
	}
}

void DScene::changeBorderWidth(qreal width)
{
	qDebug() << "change border width";
	for(DShapeBase* shape : getSelectedShapes())
	{
		QPen npen = shape->pen();
		npen.setWidthF(width);
		shape->setPen(npen);
	}
}

void DScene::changeBorderColor(QColor color)
{
	qDebug() << "change border color";
	for(DShapeBase* shape : getSelectedShapes())
	{
		QPen npen = shape->pen();
		npen.setColor(color);
		shape->setPen(npen);
	}
}

void DScene::changeFillType(Qt::BrushStyle brushstyle)
{
	qDebug() << "change fill type";
	for(DShapeBase* shape : getSelectedShapes())
	{
        QBrush nbrush = shape->brush();
        nbrush.setStyle(brushstyle);
		shape->setBrush(nbrush);
	}
}

void DScene::changeFillColor(QColor color)
{
	qDebug() << "change border width";
	for(DShapeBase* shape : getSelectedShapes())
	{
		QBrush nbrush = shape->brush();
        nbrush.setStyle(Qt::SolidPattern);
		nbrush.setColor(color);
		shape->setBrush(nbrush);
	}
}

void DScene::changeFillPic(QPixmap pixmap)
{
    qDebug() << "change border pic";
	QList<DShapeBase*> shapes = getSelectedShapes();
    for(DShapeBase* shape : shapes)
    {
        QBrush nbrush = shape->brush();
        nbrush.setTexture(pixmap);
        shape->setBrush(nbrush);
    }
}

QSet<DTextBase *> DScene::getTextBases()
{
    QSet<DTextBase *> texts;
    QList<QGraphicsItem *> items = selectedItems();
    for(QGraphicsItem *item : items) {
        DTextItem *textitem = dynamic_cast<DTextItem *>(item);
        if(textitem != nullptr) {
            texts.insert(&(textitem->textBase));
            continue;
        }
        DShapeBase *shape = dynamic_cast<DShapeBase *>(item);
        if(shape != nullptr) {
            texts.insert(&(shape->textItem->textBase));
            continue;
        }
        DTextBase *textbase = dynamic_cast<DTextBase *>(item);
        if(textbase != nullptr) {
            // qDebug() << "text not null";
            texts.insert(textbase);
        }
    }
    return texts;
}

void DScene::changeTextColor(QColor color)
{
    QSet<DTextBase *> texts = getTextBases();
    QTextCharFormat charformat;
    charformat.setForeground(color);
    for(DTextBase *tbase : texts) {
        QTextCursor cursor(tbase->document());
        cursor.movePosition(QTextCursor::Start);
        while(!cursor.isNull() && !cursor.atEnd()) {
            cursor.select(QTextCursor::BlockUnderCursor);
            cursor.mergeCharFormat(charformat);
            cursor.movePosition(QTextCursor::NextBlock);
        }
    }
}

void DScene::changeTextFont(QFont font)
{
    QSet<DTextBase *> texts = getTextBases();
    for(DTextBase *tbase : texts) {
        tbase->document()->setDefaultFont(font);
    }
}

void DScene::setBackground(QString path)
{
    QPixmap bg(path);
    setBackgroundBrush(bg);
}

//==============================================================================

void DScene::itemTopAlign()
{
	QList<DShapeBase*> shapes = getNoparentSelectedShapes();
	if(shapes.size() <= 1) return;

	auto getTop = [](DShapeBase* shape) -> qreal {
		return shape->mapRectToScene(shape->sizeRect()).top();
	};

	qreal topMin = getTop(shapes[0]);
	for(DShapeBase* shape : shapes)
		topMin = qMin(topMin, getTop(shape));

	for(DShapeBase* shape : shapes)
	{
		QPointF p = shape->pos();
		p.setY(p.y() + topMin - getTop(shape));
		shape->setPos(p);
	}
}

void DScene::itemBottomAlign()
{
	QList<DShapeBase*> shapes = getNoparentSelectedShapes();
	if(shapes.size() <= 1) return;

	auto getBottom = [](DShapeBase* shape) -> qreal {
		return shape->mapRectToScene(shape->sizeRect()).bottom();
	};

	qreal bottomMax = getBottom(shapes[0]);
	for(DShapeBase* shape : shapes)
		bottomMax = qMax(bottomMax, getBottom(shape));

	for(DShapeBase* shape : shapes)
	{
		QPointF p = shape->pos();
		p.setY(p.y() + bottomMax - getBottom(shape));
		shape->setPos(p);
	}
}

void DScene::itemLeftAlign()
{
	QList<DShapeBase*> shapes = getNoparentSelectedShapes();
	if(shapes.size() <= 1) return;

	auto getLeft = [](DShapeBase* shape) -> qreal {
		return shape->mapRectToScene(shape->sizeRect()).left();
	};

	qreal leftMin = getLeft(shapes[0]);
	for(DShapeBase* shape : shapes)
		leftMin = qMin(leftMin, getLeft(shape));

	for(DShapeBase* shape : shapes)
	{
		QPointF p = shape->pos();
		p.setX(p.x() + leftMin - getLeft(shape));
		shape->setPos(p);
	}
}

void DScene::itemRightAlign()
{
	QList<DShapeBase*> shapes = getNoparentSelectedShapes();
	if(shapes.size() <= 1) return;

	auto getRight = [](DShapeBase* shape) -> qreal {
		return shape->mapRectToScene(shape->sizeRect()).right();
	};

	qreal rightMax = getRight(shapes[0]);
	for(DShapeBase* shape : shapes)
		rightMax = qMax(rightMax, getRight(shape));

	for(DShapeBase* shape : shapes)
	{
		QPointF p = shape->pos();
		p.setX(p.x() + rightMax - getRight(shape));
		shape->setPos(p);
	}
}

void DScene::itemHorizAlign()
{
	QList<DShapeBase*> shapes = getNoparentSelectedShapes();
	if(shapes.size() <= 1) return;

	auto getCentX = [](DShapeBase* shape) -> qreal {
		return shape->mapRectToScene(shape->sizeRect()).center().x();
	};

	qreal rightMax = getCentX(shapes[0]);
	qreal leftMin = getCentX(shapes[0]);
	for(DShapeBase* shape : shapes)
	{
		rightMax = qMax(rightMax, getCentX(shape));
		leftMin = qMin(leftMin, getCentX(shape));
	}

	qreal target = (rightMax + leftMin) / 2;
	for(DShapeBase* shape : shapes)
	{
		QPointF p = shape->pos();
		p.setX(p.x() + target - getCentX(shape));
		shape->setPos(p);
	}
}

void DScene::itemVertiAlign()
{
	QList<DShapeBase*> shapes = getNoparentSelectedShapes();
	if(shapes.size() <= 1) return;

	auto getCentY = [](DShapeBase* shape) -> qreal {
		return shape->mapRectToScene(shape->sizeRect()).center().y();
	};

	qreal bottomMax = getCentY(shapes[0]);
	qreal topMin = getCentY(shapes[0]);
	for(DShapeBase* shape : shapes)
	{
		bottomMax = qMax(bottomMax, getCentY(shape));
		topMin = qMin(topMin, getCentY(shape));
	}

	qreal target = (bottomMax + topMin) / 2;
	for(DShapeBase* shape : shapes)
	{
		QPointF p = shape->pos();
		p.setY(p.y() + target - getCentY(shape));
		shape->setPos(p);
	}
}

void DScene::itemHorizEven()
{
	QList<DShapeBase*> shapes = getNoparentSelectedShapes();
	if(shapes.size() <= 1) return;

	auto getCentX = [](DShapeBase* shape) -> qreal {
		return shape->mapRectToScene(shape->sizeRect()).center().x();
	};

	std::sort(shapes.begin(), shapes.end(),
			  [&](DShapeBase* x, DShapeBase* y) -> bool {
		return getCentX(x) < getCentX(y);
	});

	qreal rightMax = getCentX(shapes[0]);
	qreal leftMin = getCentX(shapes[0]);
	for(DShapeBase* shape : shapes)
	{
		rightMax = qMax(rightMax, getCentX(shape));
		leftMin = qMin(leftMin, getCentX(shape));
	}

	qreal target = leftMin;
	qreal step = (rightMax - leftMin) / (shapes.size() - 1);
	for(DShapeBase* shape : shapes)
	{
		QPointF p = shape->pos();
		p.setX(p.x() + target - getCentX(shape));
		shape->setPos(p);
		target += step;
	}
}

void DScene::itemVertiEven()
{
	QList<DShapeBase*> shapes = getNoparentSelectedShapes();
	if(shapes.size() <= 1) return;

	auto getCentY = [](DShapeBase* shape) -> qreal {
		return shape->mapRectToScene(shape->sizeRect()).center().y();
	};

	std::sort(shapes.begin(), shapes.end(),
			  [&](DShapeBase* x, DShapeBase* y) -> bool {
		return getCentY(x) < getCentY(y);
	});

	qreal bottomMax = getCentY(shapes[0]);
	qreal topMin = getCentY(shapes[0]);
	for(DShapeBase* shape : shapes)
	{
		bottomMax = qMax(bottomMax, getCentY(shape));
		topMin = qMin(topMin, getCentY(shape));
	}

	qreal target = topMin;
	qreal step = (bottomMax - topMin) / (shapes.size() - 1);
	for(DShapeBase* shape : shapes)
	{
		QPointF p = shape->pos();
		p.setY(p.y() + target - getCentY(shape));
		shape->setPos(p);
		target += step;
	}
}

QPointF DScene::getAutoAlignItemPos(DShapeBase* item)
{
	QList<QGraphicsItem*> items = this->items(view->mapToScene(view->rect()));
	QList<DShapeBase*> shapes = DTool::itemToShape(items);
	DTool::filterNoparent(shapes);

	if(magLineH->scene()) removeItem(magLineH);
	if(magLineV->scene()) removeItem(magLineV);

	QPointF pos = item->pos();
	QRectF rc = item->mapRectToScene(item->sizeRect());

	int flag = 0;
	for(DShapeBase* shape : shapes)
		if(shape != item) flag = 1;
	if(!flag) return pos;

	qreal mindistx = qrealMax, posx, linex;
	qreal mindisty = qrealMax, posy, liney;
	QRectF vrc = view->mapToScene(view->rect()).boundingRect();

	auto updateValueX = [&](qreal val, qreal base) {
		qreal dist = abs(val - base);
		if(dist > mindistx) return;
		mindistx = dist; posx = pos.x() + val - base; linex = val;
	};
	auto updateValueY = [&](qreal val, qreal base) {
		qreal dist = abs(val - base);
		if(dist > mindisty) return;
		mindisty = dist; posy = pos.y() + val - base; liney = val;
	};

	for(DShapeBase* shape : shapes)
	{
		if(shape == item) continue;
		QRectF nrc = shape->mapRectToScene(shape->sizeRect());
		updateValueY(nrc.center().y(), rc.center().y());
		updateValueY(nrc.top(), rc.top());
		updateValueY(nrc.bottom(), rc.bottom());
		updateValueY(nrc.top(), rc.bottom());
		updateValueY(nrc.bottom(), rc.top());
		updateValueX(nrc.center().x(), rc.center().x());
		updateValueX(nrc.left(), rc.left());
		updateValueX(nrc.right(), rc.right());
		updateValueX(nrc.left(), rc.right());
		updateValueX(nrc.right(), rc.left());
	}

	if(mindistx <= maxMagLineDist)
	{
		pos.setX(posx);
		magLineV->setLine(QLineF({linex, vrc.top()},
								 {linex, vrc.bottom()}));
		addItem(magLineV);
	}
	if(mindisty <= maxMagLineDist)
	{
		pos.setY(posy);
		magLineH->setLine(QLineF({vrc.left(), liney},
								 {vrc.right(), liney}));
		addItem(magLineH);
	}

	return pos;
}

void DScene::setAutoAlign(bool active)
{
	autoAlign = active;
}

//==============================================================================

void DScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF p;
	QList<QGraphicsItem *> items;
	QGraphicsItem* item;

	if(event->button() != Qt::LeftButton) goto mousePressEventPass;

	p = event->scenePos();
	qDebug() << "press pos: " << p;

	// ======== inserting item ========
	if(insert_state == DConst::INSERT_SHAPE || insert_state == DConst::INSERT_LINE
	   || insert_state == DConst::INSERT_TEXT)
	{
		event->accept();
		modifiedShape->setSelected(true);
        modifiedShape->setZValue(TOTAL_MAX_Z_VALUE++);          // 累加z值
		addItem(modifiedShape);
		if(insert_state == DConst::INSERT_SHAPE || insert_state == DConst::INSERT_TEXT)
		{
			DShapeBase* shape = dynamic_cast<DShapeBase*>(modifiedShape);
			shape->setInsertItem();
			shape->setPos(p + QPointF(shape->sizeRect().width()/2, shape->sizeRect().height()/2));
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
		inter_state = DConst::SIZE;
		insert_state = insert_state + 1;
		return;
	}

	// ======== check for interaction =======
	items = this->items(p);
	if(items.empty()) goto mousePressEventPass;

	item = items.first();
	if(!item->isSelected()) goto mousePressEventPass;

	if((modifiedShape = dynamic_cast<DAbstractBase*>(item)) != nullptr)
	{
		if(modifiedShape->checkInterPoint(p))
			inter_state = modifiedShape->setInterPoint(p);
		else
		{
			inter_state = DConst::NONE;
			modifiedShape = nullptr;
		}
	}

mousePressEventPass:
	QGraphicsScene::mousePressEvent(event);

	if(!this->items(p).empty() && !selectedItems().empty())
		drag_state = DConst::DRAGING;
	else
		drag_state = DConst::NONE;
}


void DScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF p = event->scenePos();

	//=========== check for magpoint ===========
	if(showMagedItem)
	{
		showMagedItem->setShowMagPoint(false);
		showMagedItem = nullptr;
	}

	MagPoint *magPoint = nullptr;
	if((inter_state == DConst::SIZE && modifiedShape && modifiedShape->isLine())
	   || insert_state == DConst::INSERT_LINE)
	{
		DAbstractBase* shape = getMagItemOnPoint(p);
		if(shape)
		{
			magPoint = shape->getMagPoint(p);
			shape->setShowMagPoint(true);
			showMagedItem = shape;
		}
	}

	//============== modify interacting shape =============
	if(inter_state != DConst::NONE && modifiedShape)
	{
		event->accept();
		modifiedShape->interToPoint(p, magPoint);
		if(insert_state == DConst::AFTER_INSERT_SHAPE)
		{
			DShapeBase *shape = dynamic_cast<DShapeBase*>(modifiedShape);
			QRectF rc = shape->sizeRect();
			rc.setRect(rc.left()*0.75, rc.top()*0.75, rc.width()*0.75, rc.height()*0.75);
			shape->textItem->sizeToRect(rc);
		}
		return;
	}

	QGraphicsScene::mouseMoveEvent(event);

	//============= auto align =============
	if(!autoAlign) return;
	if(drag_state == DConst::NONE || selectedItems().size() > 1)
		return;
	QList<DShapeBase*> shapes = getSelectedShapes();
	if(shapes.empty()) return;

	shapes[0]->setPos(getAutoAlignItemPos(shapes[0]));
}

void DScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if(event->button() != Qt::LeftButton) goto mouseReleaseEventPass;

    if((insert_state != DConst::NONE)
        || SHOT_STATE == DConst::CHANGED){
        shot();
		Inspector::instance()->checkAll();
	}

	if(drag_state != DConst::NONE)
	{
		for(DLineBase* line : getSelectedLines())
			line->checkAutoUnlinkLine();
	}

	insert_state = DConst::NONE;
	inter_state = DConst::NONE;
	drag_state = DConst::NONE;
	if(magLineH->scene()) removeItem(magLineH);
	if(magLineV->scene()) removeItem(magLineV);
	modifiedShape = nullptr;

mouseReleaseEventPass:
	QGraphicsScene::mouseReleaseEvent(event);
}

void DScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	menu->popup(event->screenPos());
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
