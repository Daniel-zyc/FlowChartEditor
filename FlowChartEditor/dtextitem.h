#pragma once

#include "global.h"

#include <QGraphicsTextItem>
#include <QFocusEvent>
#include <QGraphicsSceneMouseEvent>
#include <QTransform>

class DTextItem : public QGraphicsTextItem
{
	Q_OBJECT

public:
	enum { Type = DConst::DTextItemType };
	DTextItem(QGraphicsItem *parent = nullptr);
	DTextItem(const QString &text, QGraphicsItem *parent = nullptr);

	int type() const override { return Type; }

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

	void startEdit();
	void endEdit();

protected:
	void focusOutEvent(QFocusEvent *event) override;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

protected:
	void init();
	void focusToCenter();

private:
	QPointF curCent;
};

