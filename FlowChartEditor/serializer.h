#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QGraphicsScene>

#include "magpoint.h"
#include "dlinebase.h"
#include "drectitem.h"
#include "dpolygonbase.h"

/**
 * @brief The Serializer class
 * 序列化管理
 */
class Serializer
{
public:
    static Serializer& instance(){
        static Serializer instance;
        return instance;
    }

    // 原地址 -- LineBase
    QMap<qintptr, DLineBase*> PtrToLineBase;
    // 原地址 -- QGraphicsItem
    QMap<qintptr, QGraphicsItem*> PtrToQGraphicsItem;
    // 原地址 -- TextItem
    QMap<qintptr, DTextItem*> PtrToTextItem;
    // 原地址 -- MagPoint
    QMap<qintptr, MagPoint*> PtrToMagPoint;

    // MagPoint -- lines原地址
    QMap<MagPoint*, qintptr> MagPointToLinesPtr;
    // DShapeBase -- TextItem原地址
    QMap<DShapeBase*, qintptr> DShapeBaseToTextItem;

    // LineBase -- beginMagPoint原地址
    QMap<DLineBase*, qintptr> LineBaseToBeginMagPonint;
    // LineBase -- endMagPoint 原地址
    QMap<DLineBase*, qintptr> LineBaseToEndMagPoint;

    void serializeSceneItems(QDataStream &out, QGraphicsScene *scene);

    void deserializeSceneItems(QDataStream &in, QGraphicsScene *scene);

private:
    Serializer() = default;
    Serializer(const Serializer&) = delete;
    Serializer& operator=(const Serializer&) = delete;

    void linkAll();

    void clearMap();

    void printMapSize();
};

#endif // SERIALIZER_H
