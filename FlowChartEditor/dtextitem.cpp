#include "dtextitem.h"
#include "dshapebase.h"
#include "magpoint.h"

#include <QTextDocument>
#include <QStyleOptionGraphicsItem>

DTextBase::DTextBase(QGraphicsItem *parent)
	: QGraphicsTextItem(parent)
{
	setFlags(QGraphicsItem::ItemIsSelectable);
}

DTextBase::DTextBase(const QString &text, QGraphicsItem *parent)
	: DTextBase(parent)
{
    this->document()->setPlainText(text);
}

void DTextBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	focusToCenter();
	QStyleOptionGraphicsItem nOption(*option);
	nOption.state &= ~QStyle::State_HasFocus;
	nOption.state &= ~QStyle::State_Selected;
	QGraphicsTextItem::paint(painter, &nOption, widget);
}

void DTextBase::focusOutEvent(QFocusEvent *event)
{
	endEdit();
	QGraphicsTextItem::focusOutEvent(event);
}

void DTextBase::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsTextItem::mousePressEvent(event);
}

void DTextBase::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	startEdit();
	QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void DTextBase::startEdit()
{
	parentItem()->setSelected(true);
	setTextInteractionFlags(Qt::TextEditorInteraction);
}

void DTextBase::endEdit()
{
	setTextInteractionFlags(Qt::NoTextInteraction);
}

void DTextBase::focusToCenter()
{
	QPointF cent = this->boundingRect().center();
	if(cent == curCent) return;

	this->setTransformOriginPoint(cent);
	QTransform trans; trans.translate(-cent.x(), -cent.y());
	this->setTransform(trans);
	curCent = cent;
}

void DTextBase::serialize(QDataStream &out) const{
    out << curCent;
}

void DTextBase::deserialize(QDataStream &in){
    in >> curCent;
}
//==============================================================================

DTextItem::DTextItem(QGraphicsItem *parent)
	: DShapeBase(parent) {}

DTextItem::DTextItem(const QString &text, QGraphicsItem *parent)
	: DShapeBase(parent)
{
	textBase.document()->setPlainText(text);
}

DTextItem::DTextItem(qreal w, qreal h, const QString &text, QGraphicsItem *parent)
	: DShapeBase(parent)
{
	textBase.document()->setPlainText(text);

	mags->push_back(new MagPoint(this));
	mags->push_back(new MagPoint(this));
	mags->push_back(new MagPoint(this));
	mags->push_back(new MagPoint(this));
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
	QPainterPath pth;
	pth.addRect(rect);
	return pth;
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
	Q_UNUSED(p); Q_UNUSED(id);
	return;
}

void DTextItem::deleteMagPoint()
{
	mags->clear();
}

void DTextItem::setRect(const QRectF &nrect)
{
	rect = nrect;
	textBase.setTextWidth(rect.width());
	sizeRectUpdated();
	updateMagPoint();
}

void DTextItem::serialize(QDataStream &out) const{
    DShapeBase::serialize(out);

    textBase.serialize(out);

    out << rect;
}

void DTextItem::deserialize(QDataStream &in){
    DShapeBase::deserialize(in);

    textBase.deserialize(in);

    in >> rect;
}
