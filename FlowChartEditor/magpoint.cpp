#include "magpoint.h"
#include "dlinebase.h"
#include "dabstractbase.h"
#include "serializer.h"

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
	for(DLineBase* l : *lines) if(l == line) continue;
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

//=====================================================
// 序列化与反序列化
void MagPoint::serialize(QDataStream &out) const{
    out << reinterpret_cast<qintptr>(this);
    out << pos;
	// qintptr parentPtr = (parent != nullptr) ? reinterpret_cast<qintptr>(parent) : qintptr(-1);
	// out << parentPtr;

    if(lines == nullptr){
        out << static_cast<quint32>(0);
    }else{
        out << static_cast<quint32>(lines->size());
        for(DLineBase * line: *lines){
            out << reinterpret_cast<qintptr>(line);
        }
    }
}

void MagPoint::deserialize(QDataStream &in){
	qintptr thisPtr; in >> thisPtr;
    Serializer::instance().PtrToMagPoint.insert(thisPtr, this);

    in >> this->pos;

	// qintptr parentPtr;
	// in >> parentPtr;

	// if(parentPtr != -1) Serializer::instance().MagPointToParentPtr.insert(this, parentPtr);

	quint32 lineCount; in >> lineCount;
    for(quint32 i = 0; i < lineCount; i++)
    {
		qintptr linePtr; in >> linePtr;
        Serializer::instance().MagPointToLinesPtr.insert(this,linePtr);
    }
}

void MagPoint::linkParent(QGraphicsItem * item){
    parent = item;
}

void MagPoint::linkLine(DLineBase *line){
    lines->append(line);
}
