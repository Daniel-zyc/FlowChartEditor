#include "serializer.h"
#include "dabstractbase.h"
#include "dtextitem.h"
#include "dellitem.h"
#include "droundrectitem.h"
#include "dlineitem.h"
#include "dtriitem.h"

void Serializer::serializeSceneItems(QDataStream &out, QGraphicsScene *scene){
    QList<QGraphicsItem *> items = scene->items();
    // 输出数量
    qint32 size = 0;
    for (QGraphicsItem *item : items) {
        if (dynamic_cast<DAbstractBase*>(item)) {
            size++;
        }
    }
    out << size;
    for (QGraphicsItem* item : items) {
        if (auto* abstractItem = dynamic_cast<DAbstractBase*>(item)) {
			out << abstractItem->type();
            abstractItem->serialize(out);
        } else if(auto* abstractItem = dynamic_cast<MagPoint*>(item)){
            qDebug() << "serializing MagPoint";
        }else {
            qDebug() << "Item is not derived from DAbstractBase";
        }
    }
}

void Serializer::printMapSize(){
    qDebug() << "size of six map"
             << PtrToLineBase.size()
             << PtrToQGraphicsItem.size()
             << MagPointToLinesPtr.size();
}

void Serializer::deserializeSceneItems(QDataStream &in, QGraphicsScene *scene) {
    scene->clear();  // 清除现有图形项

	clearMap();

    qint32 size;
    in >> size;
    qDebug() << "反序列化项的数量:" << size;

    for (qint32 i = 0; i < size; ++i) {
        int type;
        in >> type;
        QGraphicsItem *item = nullptr;

        switch (type) {
        case DTextItem::Type:
            item = new DTextItem();
            break;
        case DRectItem::Type:
            item = new DRectItem();
            break;
        case DEllItem::Type:
            item = new DEllItem();
            break;
        case DRoundRectItem::Type:
            item = new DRoundRectItem();
            break;
        case DLineItem::Type:
            item = new DLineItem();
            break;
        case DTriItem::Type:
            item = new DTriItem();
            break;
        default:
            qDebug() << "unknown type:" << type;
            continue;
        }

        if (item) {
            // 反序列化图形项的状态
            DAbstractBase *abstractItem = dynamic_cast<DAbstractBase*>(item);
			if (abstractItem) {
				abstractItem->deserialize(in);
            } else {
                delete item;
                qDebug() << "fail to deserialize";
            }
        }
    }

	linkAll();

    for(auto it = PtrToQGraphicsItem.cbegin(); it != PtrToQGraphicsItem.cend(); ++it){
		if(it.value()->parentItem() != nullptr) continue;
		scene->addItem(it.value());
    }
}



void Serializer::linkAll(){

    for(auto it = MagPointToLinesPtr.cbegin(); it != MagPointToLinesPtr.cend(); ++it){
        MagPoint* magPoint = it.key();
        qintptr linePtr = it.value();
        if(PtrToLineBase.contains(linePtr)){
            magPoint->linkLine(PtrToLineBase[linePtr]);
        }
    }

    for(auto it = DShapeBaseToTextItem.cbegin(); it != DShapeBaseToTextItem.cend(); ++it){
        DShapeBase * dshapeBase = it.key();
        qintptr textItemPtr = it.value();
        if(PtrToTextItem.contains(textItemPtr)){
            dshapeBase->linkTextItem(PtrToTextItem[textItemPtr]);
        }
    }

    for(auto it = LineBaseToBeginMagPonint.cbegin(); it != LineBaseToBeginMagPonint.cend(); ++ it){
        DLineBase * dlineBase = it.key();
        qintptr beginMag = it.value();
        if(PtrToMagPoint.contains(beginMag)){
            dlineBase->linkBegin(PtrToMagPoint[beginMag]);
        }
    }

    for(auto it = LineBaseToEndMagPoint.cbegin(); it != LineBaseToEndMagPoint.cend(); ++ it){
        DLineBase * dlineBase = it.key();
        qintptr endMag = it.value();
        if(PtrToMagPoint.contains(endMag)){
            dlineBase->linkEnd(PtrToMagPoint[endMag]);
        }
    }
    return;
}

void Serializer::clearMap(){
    PtrToLineBase.clear();
    PtrToQGraphicsItem.clear();
	PtrToTextItem.clear();
	PtrToMagPoint.clear();
	DShapeBaseToTextItem.clear();
	LineBaseToBeginMagPonint.clear();
	LineBaseToEndMagPoint.clear();
    MagPointToLinesPtr.clear();
}
