#include "global.h"
#include "dclass/base/dabstractbase.h"
#include "dclass/base/dshapebase.h"

#include <cmath>

QString FILE_PATH = "";

QSet<int> registeredTypes = QSet<int>(
			{
				// shape
				DRectItemType,
				DRoundRectItemType,
				DEllItemType,
				DTriItemType,
				DDiaItemType,
				DParagramItemType,
				DTrapItemType,
                DPentagonItemType,

				// shape for flowchart
				DFDocumentItemType,
				DFStartEndItemType,
				DFManualOperateItemType,
				DFInternalStoreItemType,
				DFPrepareItemType,
				DFProcessItemType,
				DFOptionalProcessItemType,
				DFConditionItemType,
				DFDataItemType,
				DFNodeItemType,
				DFInformationItemType,
				DFManualInputItemType,
				DFPredefineItemType,
				DFDelayItemType,
				DFOrItemType,
				DFSummaryconnItemType,
				DFCardItemType,
				DFCompareItemType,
				DFMergeItemType,
				DFOffPageItemType,
				DFSortItemType,
				DFStoreDataItemType,
				DFShowItemType,
				DFDirectStorageItemType,
				DFDiskItemType,
				DFMultiDocItemType,
				DFOrderStorageItemType,

				// text
				DTextItemType,

				// line
				DLineItemType,
				DCurveLineItemType,
				DPolyLineItemType
			});

int TOTAL_MAX_Z_VALUE = 1;

int SHOT_STATE = DConst::UNCHANGED;

QBrush globalShapeBrush = defaultBrush;
QPen globalShapePen = defaultPen;
QPen globalLinePen = defaultPen;

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

void DTool::moveItems(const QList<QGraphicsItem *> &items,const QPointF copyCenterPos, const QPointF cursorPos)
{
    QPointF delta = cursorPos - copyCenterPos;
	QSet<QGraphicsItem*> S;
	for (QGraphicsItem* item : items) S.insert(item);
	for (QGraphicsItem* item : items)
	{
		if (item == nullptr || S.contains(item->parentItem())) continue;
        QPointF curPos = item->pos();
        item->setPos(curPos + delta);
	}
}

void DTool::filterRootBases(QList<QGraphicsItem*>& items)
{
    QSet<QGraphicsItem*> S;
    for (QGraphicsItem* item : items) {
        S.insert(item);
    }

	for (int i = 0; i < items.size(); i++)
	{
		if (items[i] == nullptr || !isAbstract(items[i]->type())
			|| S.contains(items[i]->parentItem()))
		{
			qSwap(items[i], items.back());
			items.pop_back();
			i--;
		}
	}
}

void DTool::filterBases(QList<QGraphicsItem*>& items)
{
	for (int i = 0; i < items.size(); i++)
	{
		if(items[i] == nullptr || !isAbstract(items[i]->type()))
		{
			qSwap(items[i], items.back());
			items.pop_back();
			i--;
		}
	}
}

QList<DAbstractBase*> DTool::itemsToBases(const QList<QGraphicsItem*> &items)
{
	QList<DAbstractBase*> bases;
	for(QGraphicsItem* item : items)
		if(item && isAbstract(item->type()))
			bases.push_back(dynamic_cast<DAbstractBase*>(item));
	return bases;
}


QList<DShapeBase*> DTool::itemToShape(const QList<QGraphicsItem*>& items)
{
	QList<DShapeBase*> bases;
	for(QGraphicsItem* item : items)
		if(item && isShape(item->type()))
			bases.push_back(dynamic_cast<DShapeBase*>(item));
	return bases;
}


void DTool::filterNoparent(QList<DShapeBase*>& items)
{
	for (int i = 0; i < items.size(); i++)
		if(!items[i] || items[i]->parentItem())
		{
			qSwap(items[i], items.back());
			items.pop_back();
			i--;
		}
}

void DTool::normalizeZValues(QList<QGraphicsItem *> &items) {
    DTool::filterRootBases(items);

    std::sort(items.begin(), items.end(), [](QGraphicsItem *a, QGraphicsItem *b) {
        return a->zValue() < b->zValue();
    });

    qreal newZValue = 1;
    for (QGraphicsItem *item : items) {
        item->setZValue(newZValue);
        ++newZValue;
    }
    TOTAL_MAX_Z_VALUE = newZValue;
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
	return QGraphicsItem::UserType <= type && type != DTextBaseType;
}

bool DTool::isFlowChartShape(int type)
{
	return QGraphicsItem::UserType + 200 <= type && type < QGraphicsItem::UserType + 300;
}

int DTool::getErrorLevel(int type){
    return type > 100;
}
