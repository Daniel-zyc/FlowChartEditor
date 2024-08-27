#include "dtextitem.h"

DTextItem::DTextItem(QGraphicsItem *parent)
	: QGraphicsTextItem(parent)
{
	init();
}

DTextItem::DTextItem(const QString &text, QGraphicsItem *parent)
	: QGraphicsTextItem(text, parent)
{
	init();
}

void DTextItem::startEdit()
{
	if(textInteractionFlags() == Qt::NoTextInteraction)
		setTextInteractionFlags(Qt::TextEditorInteraction);
}

void DTextItem::endEdit()
{
	setTextInteractionFlags(Qt::NoTextInteraction);
}

void DTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	focusToCenter();
	QGraphicsTextItem::paint(painter, option, widget);
}

void DTextItem::focusOutEvent(QFocusEvent *event)
{
	endEdit();
	QGraphicsTextItem::focusInEvent(event);
}

void DTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	startEdit();
	QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void DTextItem::init()
{
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsMovable);
}

void DTextItem::focusToCenter()
{
	QPointF cent = this->boundingRect().center();
	if(cent == curCent) return;

	this->setTransformOriginPoint(cent);
	QTransform trans; trans.translate(-cent.x(), -cent.y());
	this->setTransform(trans);
	curCent = cent;
}