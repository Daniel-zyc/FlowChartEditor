#include "magpoint.h"
#include "dlinebase.h"
#include "dabstractbase.h"

MagPoint::MagPoint(QGraphicsItem* parent)
	: parent(parent)
{
	lines = new QList<DLineBase*>();
}

MagPoint::MagPoint(const QPointF &p, QGraphicsItem* parent)
	: pos(p), parent(parent) {}

MagPoint::MagPoint(qreal x, qreal y, QGraphicsItem* parent)
	: MagPoint({x, y}, parent) {}

MagPoint::~MagPoint()
{
	delete lines;
}

void MagPoint::updateLines() const
{
	for(DLineBase *line : *lines) line->updatePosition();
}

void MagPoint::addLine(DLineBase *line)
{
	lines->append(line);
}

void MagPoint::deleteLine(DLineBase *line)
{
	lines->removeAll(line);
}

void MagPoint::deleteAllLines()
{
	lines->clear();
}

QPointF MagPoint::mapToItem(QGraphicsItem *item)
{
	return parent->mapToItem(item, pos);
}

