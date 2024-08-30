#include "serializer.h"

template <typename T>
void Serializer::serializeMap(QDataStream &out, const QMap<qintptr, T*> &map) const{
    out << static_cast<quint32>(map.size());
    for(auto it = map.constBegin(); it != map.constEnd(); ++it){
        out << it.key();
        out << reinterpret_cast<qintptr>(it.value());
    }
}

template <typename T>
void Serializer::serializeMap(QDataStream &out,const QMap<T*, qintptr> &map)const{
    out << static_cast<quint32>(map.size());
    for(auto it = map.constBegin(); it != map.constEnd(); ++it){
        out << reinterpret_cast<qintptr>(it.key());
        out << it.value();
    }
}

template <typename T>
void Serializer::deserializeMap(QDataStream &in, QMap<qintptr, T*> &map){
    quint32 size;
    in >> size;
    for(quint32 i = 0; i< size; ++i){
        qintptr key;
        qintptr valuePtr;
        in >> key >> valuePtr;
        map.insert(key, reinterpret_cast<T*>(valuePtr));
    }
}

template <typename T>
void Serializer::deserializeMap(QDataStream &in, QMap<T*, qintptr> &map){
    quint32 size;
    in >> size;
    for(quint32 i = 0; i < size; ++i){
        quintptr keyPtr;
        qintptr value;
        in >> keyPtr >> value;
        map.insert(reinterpret_cast<T*>(keyPtr), value);
    }
}

void  Serializer::serializeAllMap(QFile &file){
    if(!file.isOpen() || !file.isWritable()){
        qWarning() << "File is not open or not writable";
        return;
    }
    QDataStream out(&file);
    out.setVersion(DataStreamVersion);

    serializeAllMap(out);
}

void Serializer::serializeAllMap(QDataStream &out){
    serializeMap(out, PtrToMagPoint);
    serializeMap(out, PtrToLineBase);
    serializeMap(out, PtrToQGraphicsItem);

    serializeMap(out, MagPointToLinesPtr);
    serializeMap(out, MagPointToParentPtr);
    serializeMap(out, DAbstractBaseToMagsPtr);
}


void Serializer::deserializeAllMap(QFile &file) {
    if (!file.isOpen() || !file.isReadable()) {
        qWarning() << "File is not open or not readable";
        return;
    }
    QDataStream in(&file);
    in.setVersion(DataStreamVersion);

    deserializeAllMap(in);
}

// 反序列化所有的 QMap 从 QDataStream
void Serializer::deserializeAllMap(QDataStream &in) {
    deserializeMap(in, PtrToMagPoint);
    deserializeMap(in, PtrToLineBase);
    deserializeMap(in, PtrToQGraphicsItem);

    deserializeMap(in, MagPointToLinesPtr);
    deserializeMap(in, MagPointToParentPtr);
    deserializeMap(in, DAbstractBaseToMagsPtr);
}


void Serializer::serializeSceneItems(QDataStream &out, QGraphicsScene *scene){
    QList<QGraphicsItem *> items= scene->items();
    // 输出数量
    out << items.size();

    for (QGraphicsItem* item : items) {
        if (auto* abstractItem = dynamic_cast<DAbstractBase*>(item)) {
            // 输出类型
            out << abstractItem->type();

            switch (abstractItem->type()) {
            case DAbstractBase::Type:
                qDebug() << "serializing DAbstractBase";
                break;
            case DLineBase::Type:
                qDebug() << "serializing DLineBase";
                break;
            case DRectItem::Type:
                qDebug() << "serializing DRectItem";
                break;
            case DPolygonBase::Type:
                qDebug() << "serializing DPolygonBase";
                break;
            default:
                qDebug() << "Unknown DAbstractBase-derived item type";
                break;
            }
        } else {
            qDebug() << "Item is not derived from DAbstractBase";
        }
    }
}
