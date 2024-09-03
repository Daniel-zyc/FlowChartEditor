#include "serializer.h"
#include "dallitems.h"

void Serializer::serializeEmptyItems(QDataStream &out){
	out << (qint32)0 << (qint32)0 << (qint32)0;
}

void Serializer::filterSerializableItem(QList<QGraphicsItem*>& items)
{
	QSet<QGraphicsItem*> S;
	for (QGraphicsItem* item : items) S.insert(item);
	for (int i = 0; i < items.size(); i++)
	{
		if(items[i] == nullptr || S.contains(items[i]->parentItem())
		   || !registeredTypes.contains(items[i]->type()))
		{
			qSwap(items[i], items.back());
			items.pop_back();
		}
	}
}

void Serializer::serializeItems(QDataStream &out, QList<QGraphicsItem *> items)
{
    // qDebug() << "开始对象序列化";
    // qDebug() << "传入对象数量: " << items.size();

	filterSerializableItem(items);

    // qDebug() << "过滤不参与序列化以及父对象也会被序列化的元素后数量: " << items.size();

	QList<DShapeBase*> shapes;
	QList<DTextItem*> texts;
	QList<DLineBase*> lines;

	for (QGraphicsItem *item : items)
	{
		if(dynamic_cast<DTextItem*>(item))
		{
			texts.push_back(dynamic_cast<DTextItem*>(item));
			continue;
		}
		if(dynamic_cast<DShapeBase*>(item))
		{
			shapes.push_back(dynamic_cast<DShapeBase*>(item));
			continue;
		}
		if(dynamic_cast<DLineBase*>(item))
		{
			lines.push_back(dynamic_cast<DLineBase*>(item));
			continue;
		}
	}

    // qDebug() << "序列化中 shape 的数量: " << shapes.size();
	out << (qint32)shapes.size();
	for (DShapeBase *shape : shapes)
	{
		out << (qint32)shape->type();
		shape->serialize(out);
	}

    // qDebug() << "序列化中 text 的数量: " << texts.size();
	out << (qint32)texts.size();
	for (DTextItem *text : texts) text->serialize(out);

    // qDebug() << "序列化中 line 的数量: " << lines.size();
	out << (qint32)lines.size();
	for (DLineBase *line : lines)
	{
		out << (qint32)line->type();
		line->serialize(out);
	}
}

QList<QGraphicsItem *> Serializer::deserializeItems(QDataStream &in)
{
    // qDebug() << "反序列化开始";

	clearMap();

	qint32 shapeSize, textSize, lineSize;
	QList<QGraphicsItem* > data;

	int tmpcnt;

    in >> shapeSize;
    // qDebug() << "读取到的 shape 数量: " << shapeSize;
	for(tmpcnt = 0; shapeSize; shapeSize--)
	{
		qint32 type; in >> type;

		if(!registeredTypes.contains(type)) continue;

		DShapeBase *shape = nullptr;
		switch(type) {
			case DRectItemType: shape = new DRectItem(); break;
			case DRoundRectItemType: shape = new DRoundRectItem(); break;
			case DEllItemType: shape = new DEllItem(); break;
			case DTriItemType: shape = new DTriItem(); break;
			case DDiaItemType: shape = new DDiaItem(); break;
		}
		if(!shape || !shape->deserialize(in))
		{
			qDebug() << "shape 序列化失败";
			delete shape;
		}
		data.push_back(shape);
		tmpcnt++;
	}
    // qDebug() << "成功序列化 shape 数量: " << tmpcnt;

	in >> textSize;
    // qDebug() << "读取到的 text 数量: " << textSize;
	for(tmpcnt = 0; textSize; textSize--)
	{
		DTextItem *text = new DTextItem();
		if(!text->deserialize(in))
		{
			qDebug() << "text 序列化失败";
			delete text;
		}
		data.push_back(text);
		tmpcnt++;
	}
    // qDebug() << "成功序列化 text 数量: " << tmpcnt;

	in >> lineSize;
    // qDebug() << "读取到的 line 数量: " << lineSize;
	for(tmpcnt = 0; lineSize; lineSize--)
	{
		qint32 type; in >> type;

		if(!registeredTypes.contains(type)) continue;

		DLineBase *line = nullptr;
		switch(type) {
			case DLineItemType: line = new DLineItem(); break;
		}
		if(!line || !line->deserialize(in))
		{
			qDebug() << "line 序列化失败";
			delete line;
		}
		data.push_back(line);
		tmpcnt++;
	}
    // qDebug() << "成功序列化 line 数量: " << tmpcnt;

	return data;
}

void Serializer::serializeSceneItems(QDataStream &out, QGraphicsScene *scene)
{
	serializeItems(out, scene->items());
}

void Serializer::clearMap()
{
	ptrToMag.clear(); ptrToMag[0] = nullptr;
}
