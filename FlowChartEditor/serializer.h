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

    QMap<qintptr, MagPoint*> PtrToMagPoint;
    QMap<qintptr, DLineBase*> PtrToLineBase;
    QMap<qintptr, QGraphicsItem*> PtrToQGraphicsItem;

    QMap<MagPoint*, qintptr> MagPointToLinesPtr;
    QMap<MagPoint*, qintptr> MagPointToParentPtr;
    QMap<DAbstractBase*, qintptr> DAbstractBaseToMagsPtr;

private:
    Serializer() = default;
    Serializer(const Serializer&) = delete;
    Serializer& operator=(const Serializer&) = delete;

    void linkAll();

    void loadFromFile();

    void serializeSceneItems(QDataStream &out, QGraphicsScene *scene);

    //===============================================================
    template <typename T>
    void serializeMap(QDataStream &out, const QMap<qintptr, T*> &map) const;

    template <typename T>
    void serializeMap(QDataStream &out, const QMap<T*, qintptr> &map) const;

    template <typename T>
    void deserializeMap(QDataStream &in, QMap<qintptr, T*> &map);

    template <typename T>
    void deserializeMap(QDataStream &in, QMap<T*, qintptr> &map);

    void serializeAllMap(QFile &file);

    void serializeAllMap(QDataStream &out);

    void deserializeAllMap(QFile &file);

    void deserializeAllMap(QDataStream &in);
};

#endif // SERIALIZER_H
