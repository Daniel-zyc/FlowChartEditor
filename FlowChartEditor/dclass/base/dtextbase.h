#pragma once

#include "../../global.h"
#include <QGraphicsTextItem>

// 文本对象的内部数据成员，并不是 AbstractBase 继承下来的，只是用来处理文本操作
class DTextBase : public QGraphicsTextItem
{
	Q_OBJECT

public:
	enum { Type = DTextBaseType };
	DTextBase(QGraphicsItem *parent = nullptr);
	DTextBase(const QString &text, QGraphicsItem *parent = nullptr);

public:
	int type() const override { return Type; }

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

	void startEdit();
	void endEdit();

	void focusToCenter();

public:
	// 序列化文本内容
	void serialize(QDataStream &out) const;
	void deserialize(QDataStream &in);
};
