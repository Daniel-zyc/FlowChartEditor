#pragma once

#include "global.h"
#include "dshapebase.h"

#include <QGraphicsTextItem>
#include <QFocusEvent>
#include <QGraphicsSceneMouseEvent>
#include <QTransform>

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

class DTextItem : public DShapeBase
{
public:
	enum { Type = DTextItemType };
	DTextItem(QGraphicsItem *parent = nullptr);
	DTextItem(const QString &text, QGraphicsItem *parent = nullptr);
	DTextItem(qreal w, qreal h, const QString &text, QGraphicsItem *parent = nullptr);

	int type() const override { return Type; }

public:
	void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

	QRectF sizeRect() const override;
	QPainterPath shapeNormal() const override;
	void sizeToRect(QRectF nrect) override;
	void modiToPoint(QPointF p, int id) override;
	void deleteMagPoint();

protected:
	void focusOutEvent(QFocusEvent *event) override;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
	void setRect(const QRectF &nrect);
	void updateMagPoint();

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

