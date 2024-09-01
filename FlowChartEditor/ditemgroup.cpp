#include "ditemgroup.h"
#include "dshapebase.h"

DItemGroup::DItemGroup(QGraphicsItem *parent)
	: QGraphicsItemGroup(parent)
{
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable
			 | QGraphicsItem::ItemSendsGeometryChanges);
}

QRectF DItemGroup::boundingRect() const
{
	return boundRect;
}

void DItemGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	static constexpr qreal top = rotPointMargin + rotPointRadius;

	Q_UNUSED(option); Q_UNUSED(widget);

	if(!isSelected() || parentItem()) return;

	updateBoundingRect();

	painter->setBrush(groupRectBrush);
	painter->setPen(groupRectPen);
	painter->drawRect(boundingRect().adjusted(0, top, 0, 0));
}

void DItemGroup::updateChilds() const
{
	DItemGroup* group; DShapeBase* shape;
	for(QGraphicsItem* item : childItems())
	{
		group = dynamic_cast<DItemGroup*>(item);
		if(group)
		{
			group->updateChilds();
			continue;
		}

		shape = dynamic_cast<DShapeBase*>(item);
		if(shape)
		{
			shape->updateAllLinkLines();
			continue;
		}
	}
}

QVariant DItemGroup::itemChange(GraphicsItemChange change, const QVariant& value)
{
	if(change == QGraphicsItem::ItemPositionHasChanged
	   || change == QGraphicsItem::ItemRotationHasChanged
	   || change == QGraphicsItem::ItemScaleHasChanged)
	{
		updateChilds();
	}
	return value;
}

void DItemGroup::updateBoundingRect()
{
	static constexpr qreal top = rotPointMargin + rotPointRadius;
	QRectF rc = childrenBoundingRect(); rc.adjust(0, -top, 0, 0);
	if(boundRect != rc)
	{
		prepareGeometryChange();
		boundRect = rc;
	}
}
