#pragma once

#include "../../global.h"
#include "../base/dshapebase.h"
#include "../base/dtextbase.h"

#include <QGraphicsTextItem>
#include <QFocusEvent>
#include <QGraphicsSceneMouseEvent>
#include <QTransform>

class DTextItem : public DShapeBase
{
public:
	enum { Type = DTextItemType };
	DTextItem(QGraphicsItem *parent = nullptr);
	DTextItem(const QString &text, QGraphicsItem *parent = nullptr);
	DTextItem(qreal w, qreal h, const QString &text, QGraphicsItem *parent = nullptr);

public:
	int type() const override { return Type; }

	void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	QRectF sizeRect() const override;
	QPainterPath shapeNormal() const override;
	void sizeToRect(QRectF nrect) override;
	void modiToPoint(QPointF p, int id) override;

	void updateAll();
	void updateMagPoint();
	void deleteMagPoint();

    bool isTextEmpty() override;

protected:
	void focusOutEvent(QFocusEvent *event) override;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

public:
	DTextBase textBase = DTextBase(this);

private:
	QRectF rect = QRectF(0, 0, 0, 0);

public:
	/**
	 * @brief serialize
	 * @param out
	 * 序列化：DShapeBase -> textBase -> rect
	 */
	void serialize(QDataStream &out, const QGraphicsItem* fa = nullptr) const override;
	bool deserialize(QDataStream &in, QGraphicsItem* fa = nullptr) override;
};

