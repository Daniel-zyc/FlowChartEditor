#include "serializer.h"
#include "dabstractbase.h"
#include "dtextitem.h"
#include "dellitem.h"
#include "droundrectitem.h"
#include "dlineitem.h"
#include "dtriitem.h"
#include "ddocitem.h"
#include "dtrapitem.h"

void Serializer::serializeSceneItems(QDataStream &out, QList<QGraphicsItem *> items){
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

void Serializer::serializeSceneItems(QDataStream &out, QGraphicsScene *scene){
    QList<QGraphicsItem *> items = scene->items();
    serializeSceneItems(out, items);
}

void Serializer::printMapSize(){
    qDebug() << "size of six map"
             << LineBaseToBeginMagPonint.size()
            <<LineBaseToEndMagPoint.size()
             << MagPointToLinesPtr.size();
}

QList<QGraphicsItem *> Serializer::deserializeSceneItems(QDataStream &in) {
    QList<QGraphicsItem* > data;

	clearMap();

    qint32 size;
    in >> size;
    qDebug() << "反序列化项的数量:" << size;

    for (qint32 i = 0; i < size; ++i) {
        int type;
        in >> type;
        QGraphicsItem *item = nullptr;

		qDebug() << "Type: " << type;

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
        case DDocItem::Type:
            item = new DDocItem();
            break;
        case DTrapItem::Type:
            item = new DTrapItem();
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
                data.append(abstractItem);
            } else {
                delete item;
                qDebug() << "fail to deserialize";
            }
        }
    }

	linkAll();
    return data;

}



void Serializer::linkAll(){
    // printMapSize();
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

    MagPointToLinesPtr.clear();
    DShapeBaseToTextItem.clear();

    LineBaseToBeginMagPonint.clear();
    LineBaseToEndMagPoint.clear();
}
