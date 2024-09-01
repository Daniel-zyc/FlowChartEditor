#include "global.h"

#include <cmath>

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

qreal DTool::sq(qreal x)
{
	return x * x;
}

bool DTool::inCircle(const QPointF& o, qreal r, const QPointF& p)
{
	return DTool::dist(o, p) <= r;
}

void DTool::moveItems(QList<QGraphicsItem *> items){
    int randomInt = QRandomGenerator::global()->bounded(DConst::MAX_DISTANCE);
    for (QGraphicsItem* item : items) {
        if (item && item->parentItem() == nullptr) {
            QPointF currentPos = item->pos();
            item->setPos(currentPos.x() + randomInt, currentPos.y() - randomInt);
        }
    }
}
