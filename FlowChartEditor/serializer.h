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

    // 原地址 -- MagPoint
    QMap<qintptr, MagPoint*> PtrToMagPoint;
    // 原地址 -- LineBase
    QMap<qintptr, DLineBase*> PtrToLineBase;
    // 原地址 -- QGraphicsItem
    QMap<qintptr, QGraphicsItem*> PtrToQGraphicsItem;
    // 原地址 -- TextItem
    QMap<qintptr, DTextItem*> PtrToTextItem;


    // MagPoint -- lines原地址地址
    QMap<MagPoint*, qintptr> MagPointToLinesPtr;
    // MagPoint -- parent原地址
    QMap<MagPoint*, qintptr> MagPointToParentPtr;
    // DAbstractBase -- Mags原地址
    QMap<DAbstractBase*, qintptr> DAbstractBaseToMagsPtr;
    // DShapeBase -- TextItem原地址
    QMap<DShapeBase*, qintptr> DShapeBaseToTextItem;

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
