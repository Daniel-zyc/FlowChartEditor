#include "magpoint.h"
#include "dlinebase.h"
#include "dabstractbase.h"
#include "serializer.h"

MagPoint::MagPoint(QGraphicsItem* parent)
	: MagPoint({0, 0}, parent) {}

MagPoint::MagPoint(const QPointF &p, QGraphicsItem* parent)
	: pos(p), parent(parent), lines(new QSet<DLineBase*>())
{
	scenePos = mapToScene();
}

MagPoint::MagPoint(qreal x, qreal y, QGraphicsItem* parent)
	: MagPoint({x, y}, parent) {}

MagPoint::~MagPoint()
{
	delete lines;
}

void MagPoint::updateAllLinkLines()
{
	updateScenePos();
}

void MagPoint::updateScenePos()
{
	if(scenePos == mapToScene()) return;

	scenePos = mapToScene();
	for(DLineBase *line : *lines) line->updatePosition();
}

void MagPoint::addLine(DLineBase *line)
{
	lines->insert(line);
}

void MagPoint::deleteLine(DLineBase *line)
{
	lines->remove(line);
}

void MagPoint::setPos(const QPointF& p)
{
	pos = p;
	updateScenePos();
}

void MagPoint::unlinkAllLines()
{
	for(DLineBase* line : *lines) line->unlinkMag(this);
	lines->clear();
}

QPointF MagPoint::mapToItem(QGraphicsItem *item)
{
	return parent->mapToItem(item, pos);
}

QPointF MagPoint::mapToScene()
{
	return parent->mapToScene(pos);
}

//=====================================================
// 序列化与反序列化
void MagPoint::serialize(QDataStream &out) const{
	out << reinterpret_cast<qintptr>(this);
	out << pos;
}

void MagPoint::deserialize(QDataStream &in){
	qintptr thisPtr; in >> thisPtr; Serializer::instance().PtrToMagPoint.insert(thisPtr, this);
	in >> this->pos;
}
