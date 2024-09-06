#include "dtextitem.h"
#include "../base/dshapebase.h"
#include "../../magpoint.h"

#include <QTextDocument>
#include <QTextCursor>
#include <QStyleOptionGraphicsItem>

DTextItem::DTextItem(QGraphicsItem *parent)
	: DTextItem(minRectSize*0.8, minRectSize*0.8, "", parent) {}

DTextItem::DTextItem(const QString &text, QGraphicsItem *parent)
	: DTextItem(minRectSize*0.8, minRectSize*0.8, text, parent) {}

DTextItem::DTextItem(qreal w, qreal h, const QString &text, QGraphicsItem *parent)
	: DShapeBase(parent), textBase(text, this)
{
	for(int i = 0; i < 4; i++) mags->push_back(new MagPoint(this));
	rect = QRect(-w/2, -h/2, w, h);
	updateAll();
	isRotateable = (parent == nullptr);
	isScaleable = (parent == nullptr);
}

void DTextItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(painter); Q_UNUSED(option); Q_UNUSED(widget); return;
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

	(*mags)[0]->setPos({rect.left(), 0});
	(*mags)[1]->setPos({rect.right(), 0});

	(*mags)[2]->setPos({0, rect.top()});
	(*mags)[3]->setPos({0, rect.bottom()});
}

void DTextItem::sizeToRect(QRectF nrect)
{
	if(parentItem())
	{
		qreal maxWidth = dynamic_cast<DShapeBase*>(parentItem())->sizeRect().width() - sizePointRadius*2;
		if(nrect.width() > maxWidth)
		{
			nrect.setWidth(maxWidth);
			nrect.moveCenter({0, 0});
		}
	}
	rect = nrect; updateAll();
}

void DTextItem::modiToPoint(QPointF p, int id)
{
	Q_UNUSED(p); Q_UNUSED(id); return;
}

void DTextItem::deleteMagPoint()
{
	for(int i = 0; i < mags->size(); i++) delete (*mags)[i];
	mags->clear();
}

void DTextItem::updateAll()
{
	textBase.setTextWidth((rect.width() - sizePointRadius*2) / globalScale);
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
			qreal r = sizePointRadius;
			p.setX(qMin(qMax(0.0, prc.right() - r - rc.width()/2), p.x()));
			p.setX(qMax(qMin(0.0, prc.left() + r + rc.width()/2), p.x()));
			p.setY(qMin(qMax(0.0, prc.bottom() - r - rc.height()/2), p.y()));
			p.setY(qMax(qMin(0.0, prc.top() + r + rc.height()/2), p.y()));
			if(p != pos()) setPos(p);
		}
	}
	return DShapeBase::itemChange(change, value);
}

void DTextItem::serialize(QDataStream &out, const QGraphicsItem* fa) const
{
	DShapeBase::serialize(out, fa);
	textBase.serialize(out);
	out << rect;
}

bool DTextItem::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	// qDebug() << "called de text";
	if(!DShapeBase::deserialize(in, fa)) return false;

	textBase.deserialize(in);
	in >> rect;
	setRotateable(fa == nullptr);
	setScaleable(fa == nullptr);
	updateAll();
	return true;
}

bool DTextItem::isTextEmpty(){
    return textBase.toPlainText().isEmpty();
}
