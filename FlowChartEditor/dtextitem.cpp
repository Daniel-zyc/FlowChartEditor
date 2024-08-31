#include "dtextitem.h"
#include "dshapebase.h"
#include "magpoint.h"

#include <QTextDocument>
#include <QTextCursor>
#include <QStyleOptionGraphicsItem>
#include "serializer.h"

DTextBase::DTextBase(QGraphicsItem *parent)
	: DTextBase("", parent) {}

DTextBase::DTextBase(const QString &text, QGraphicsItem *parent)
	: QGraphicsTextItem(parent)
{
	document()->setPlainText(text);
	focusToCenter();
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
	out << toHtml();
	out << pos();
    return;
}

void DTextBase::deserialize(QDataStream &in){
	QString s; in >> s;
	setHtml(s);
	QPointF pos; in >> pos;
	setPos(pos);
	focusToCenter();
    return;
}

//==============================================================================

DTextItem::DTextItem(QGraphicsItem *parent)
	: DTextItem("", parent) {}

DTextItem::DTextItem(const QString &text, QGraphicsItem *parent)
	: DShapeBase(parent), textBase(text, this)
{
	for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
	setRect(textBase.boundingRect());
}

DTextItem::DTextItem(qreal w, qreal h, const QString &text, QGraphicsItem *parent)
	: DShapeBase(parent), textBase(text, this)
{
	for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
	setRect(QRect(-w/2, -h/2, w, h));
}

void DTextItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(painter); Q_UNUSED(option); Q_UNUSED(widget);
}

QRectF DTextItem::sizeRect() const
{
	return rect;
}

QPainterPath DTextItem::shapeNormal() const
{
	QPainterPath pth; pth.addRect(rect); return pth;
}

void DTextItem::updateMagPoint()
{
	if(mags->size() == 0) return;

	(*mags)[0]->pos = {rect.left(), 0};
	(*mags)[1]->pos = {rect.right(), 0};

	(*mags)[2]->pos = {0, rect.top()};
	(*mags)[3]->pos = {0, rect.bottom()};
}

void DTextItem::sizeToRect(QRectF nrect)
{
	setRect(nrect);
}

void DTextItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(p); Q_UNUSED(id); return;
}

void DTextItem::deleteMagPoint()
{
	mags->clear();
}

void DTextItem::setRect(const QRectF &nrect)
{
	rect = nrect;
	textBase.setTextWidth(rect.width() - maxPointRadius*2);
	sizeRectUpdated();
	updateMagPoint();
}

void DTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	textBase.startEdit();
	DShapeBase::mouseDoubleClickEvent(event);
}

void DTextItem::focusOutEvent(QFocusEvent *event)
{
	textBase.endEdit();
	DShapeBase::focusOutEvent(event);
}

QVariant DTextItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == QGraphicsItem::ItemSelectedHasChanged) {
		if(!isSelected()) textBase.endEdit();
	}
	return QGraphicsItem::itemChange(change, value);
}

void DTextItem::serialize(QDataStream &out) const{
    qDebug() << "DTextIetm serializing";
    DShapeBase::serialize(out);

	out << reinterpret_cast<qintptr>(this);

    textBase.serialize(out);

	out << pos();
    out << rect;
}

void DTextItem::deserialize(QDataStream &in){
    qDebug() << "DTextItem deserializing";
    DShapeBase::deserialize(in);

	qintptr thisPtr; in >> thisPtr;
	Serializer::instance().PtrToTextItem.insert(thisPtr,this);

    textBase.deserialize(in);

	QPointF pos;
	in >> pos;
	setPos(pos);
    in >> rect;
	setRect(rect);
}
