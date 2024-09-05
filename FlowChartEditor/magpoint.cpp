#include "magpoint.h"
#include "dclass/base/dlinebase.h"
#include "dclass/base/dabstractbase.h"
#include "serializer.h"
#include "dclass/base/dshapebase.h"

MagPoint::MagPoint(DShapeBase* parent)
	: MagPoint({0, 0}, parent) {}

MagPoint::MagPoint(const QPointF &p, DShapeBase* parent)
	: pos(p), parent(parent), lines(new QSet<DLineBase*>())
{
	scenePos = mapToScene();
}

MagPoint::MagPoint(qreal x, qreal y, DShapeBase* parent)
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
	for(DLineBase* line : *lines) line->unlinkMagUpdate(this);
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

std::tuple<int,int,int> MagPoint::linkedLienArrowType(){
    if(lines == nullptr){
        return std::make_tuple(0,0,0);
    }
    int in = 0,out = 0,no = 0;
    for(DLineBase *line :std::as_const(*lines)){
        switch(line->magType(this)){
            case DConst::OUT:           out ++ ;break;
            case DConst::IN:            in ++ ;break;
            case DConst::NO_IN_OR_OUT:  no ++ ; break;
            default:break;
        }
    }
    return std::make_tuple(in,out,no);
}

//=====================================================
// 序列化与反序列化
void MagPoint::serialize(QDataStream &out) const
{
	out << (qintptr)this;
	out << pos;
}

void MagPoint::deserialize(QDataStream &in, QGraphicsItem* fa)
{
	qintptr thisPtr; in >> thisPtr; Serializer::instance().ptrToMag[thisPtr] = this;
	in >> pos;
    parent = dynamic_cast<DShapeBase*>(fa);
}
