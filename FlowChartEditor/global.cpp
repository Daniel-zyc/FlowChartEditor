#include "global.h"

#include <cmath>

QSet<int> registeredTypes = QSet<int>(
			{
				// shape
				DRectItemType,
				DRoundRectItemType,
				DEllItemType,
				DTriItemType,
				DFDiaItemType,
				DParallegramItemType,
				DTrapItemType,

				// shape for flowchart
				DFDocItemType,
				DFEndItemType,
				DFManualOperateItemType,
				DFInternalStoreItemType,
				DFPrepareItemType,
				DFProcessItemType,
				DFOptionalProcessItemType,
				DFConditionItemType,
				DFDataItemType,
				DFNodeItemType,

				// text
				DTextItemType,

				// line
				DLineItemType
			});

int SHOT_STATE = DConst::UNCHANGED;

qreal DTool::degToRad(qreal deg) { return deg / 180 * DConst::PI; }

qreal DTool::radToDeg(qreal rad) { return rad / DConst::PI * 180; }

qreal DTool::degMod(qreal deg)
{
	deg = fmod(deg + 180.0, 360.0) - 180.0;
	if (deg < -180.0) deg += 360.0;
	return deg;
}

qreal DTool::dist(const QPointF& p1, const QPointF& p2)
{
	return sqrt(sq(p1.x() - p2.x()) + sq(p1.y() - p2.y()));
}

qreal DTool::sq(qreal x) { return x * x; }

bool DTool::inCircle(const QPointF& o, qreal r, const QPointF& p)
{
	return DTool::dist(o, p) <= r;
}

void DTool::moveItems(const QList<QGraphicsItem *> &items)
{
	QSet<QGraphicsItem*> S;
	for (QGraphicsItem* item : items) S.insert(item);
	for (QGraphicsItem* item : items)
	{
		if (item == nullptr || S.contains(item->parentItem())) continue;
		QPointF curPos = item->pos();
		item->setPos(curPos.x() + DConst::SHIFT_X,
					 curPos.y() - DConst::SHIFT_Y);
	}
}

void DTool::filterRootBases(QList<QGraphicsItem*>& items)
{
	QSet<QGraphicsItem*> S;
	for(QGraphicsItem* item : items) S.insert(item);
	for (int i = 0; i < items.size(); i++)
	{
		if(items[i] == nullptr || !isAbstract(items[i]->type())
		   || S.contains(items[i]->parentItem()))
		{
			qSwap(items[i], items.back());
			items.pop_back();
		}
	}
}

bool DTool::isShape(int type)
{
	return QGraphicsItem::UserType + 100 <= type && type < QGraphicsItem::UserType + 300;
}

bool DTool::isLine(int type)
{
	return QGraphicsItem::UserType + 300 <= type;
}

bool DTool::isText(int type)
{
	return QGraphicsItem::UserType + 40 <= type && type < QGraphicsItem::UserType + 100;
}

bool DTool::isAbstract(int type)
{
	return QGraphicsItem::UserType <= type;
}
