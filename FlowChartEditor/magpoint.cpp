#include "magpoint.h"

void MagPoint::updateLines() const
{
	for(DLineBase *line : lines) line->updatePosition();
}

void MagPoint::addLine(DLineBase *line)
{
	lines.append(line);
}

void MagPoint::deleteLine(DLineBase *line)
{
	lines.removeAll(line);
}

void MagPoint::deleteAllLines()
{
	lines.clear();
}

QPointF MagPoint::mapToItem(QGraphicsItem *item)
{
	return parent->mapToItem(item, x, y);
}

