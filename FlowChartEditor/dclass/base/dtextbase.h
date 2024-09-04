#pragma once

#include "../../global.h"
#include <QGraphicsTextItem>

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
	/**
	 * @brief serialize
	 * @param out
	 * 序列化：curCent
	 */
	void serialize(QDataStream &out) const;

	void deserialize(QDataStream &in);
};
