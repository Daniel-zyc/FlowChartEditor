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
	out << toHtml() << pos();
}

void DTextBase::deserialize(QDataStream &in){
	QString s; in >> s; setHtml(s);
	QPointF pos; in >> pos; setPos(pos);
	focusToCenter();
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
	updateSizePoint();
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
	if (change == QGraphicsItem::ItemPositionHasChanged) {
		if(dynamic_cast<DShapeBase*>(parentItem()))
		{
			DShapeBase* shape = dynamic_cast<DShapeBase*>(parentItem());
			QPointF p = pos(); QRectF rc = sizeRect(), prc = shape->sizeRect();
			p.setX(qMin(qMax(0.0, prc.right() - rc.width()/2), p.x()));
			p.setX(qMax(qMin(0.0, prc.left() + rc.width()/2), p.x()));
			p.setY(qMin(qMax(0.0, prc.bottom() - rc.height()/2), p.y()));
			p.setY(qMax(qMin(0.0, prc.top() + rc.height()/2), p.y()));
			if(p != pos()) setPos(p);
		}
	}
	return QGraphicsItem::itemChange(change, value);
}

void DTextItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DShapeBase::serialize(out, fa);
	textBase.serialize(out);
	out << rect;
}

bool DTextItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	if(!DShapeBase::deserialize(in, fa)) return false;

	textBase.deserialize(in);
	in >> rect;
	setRect(rect);
	return true;
}
