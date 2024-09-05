#include "dtextbase.h"

#include <QStyleOption>
#include <QTextDocument>
#include <QTextCursor>

DTextBase::DTextBase(QGraphicsItem *parent)
	: DTextBase("", parent) {}

DTextBase::DTextBase(const QString &text, QGraphicsItem *parent)
	: QGraphicsTextItem(parent)
{
	document()->setPlainText(text);
	focusToCenter();
	setScale(globalScale);
}

void DTextBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	focusToCenter();
	QStyleOptionGraphicsItem nOption(*option);
	nOption.state &= ~QStyle::State_HasFocus;
	nOption.state &= ~QStyle::State_Selected;
	QGraphicsTextItem::paint(painter, &nOption, widget);
}

void DTextBase::startEdit()
{
	setTextInteractionFlags(Qt::TextEditorInteraction);
}

void DTextBase::endEdit()
{
	QTextCursor tmpCursor = textCursor();
	tmpCursor.clearSelection();
	setTextCursor(tmpCursor);
	setTextInteractionFlags(Qt::NoTextInteraction);
}

void DTextBase::focusToCenter()
{
	QPointF cent = boundingRect().center();
	setPos(-cent);
	setTransformOriginPoint(cent);
}

void DTextBase::serialize(QDataStream &out) const{
	out << toHtml() << pos();
}

void DTextBase::deserialize(QDataStream &in){
	QString s; in >> s; setHtml(s);
	QPointF pos; in >> pos; setPos(pos);
	focusToCenter();
}
