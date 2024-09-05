#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QGraphicsScene>

#include "magpoint.h"
#include "global.h"

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

	QMap<qintptr, MagPoint*> ptrToMag;

	void serializeItems(QDataStream &out, QList<QGraphicsItem *> items);
	QList<QGraphicsItem *> deserializeItems(QDataStream &in);

    void serializeEmptyItems(QDataStream &out);
    void serializeSceneItems(QDataStream &out, QGraphicsScene *scene);

private:
    Serializer() = default;
    Serializer(const Serializer&) = delete;
    Serializer& operator=(const Serializer&) = delete;

	void filterSerializableItem(QList<QGraphicsItem*>& items);
	void clearMap();
};

#endif // SERIALIZER_H
