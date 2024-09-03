#include "inspector.h"

#include "ddiaitem.h"
#include "drectitem.h"

Inspector::Inspector(DScene* scene, DView *view):scene(scene), view(view) {}

// 菱形判定至少有一个输入两个输出
// 矩形判定至少有一个输入一个输出
// 平行四边形判定一个输入一个输出
void Inspector::check() const{
    QList<QGraphicsItem * > items = scene->items();
    for(QGraphicsItem * item : items){
        if(dynamic_cast<DDiaItem*>(item)){
            qDebug() << "找到一个菱形";
            DDiaItem *diaItem = dynamic_cast<DDiaItem*>(item);
            auto result = diaItem->getLinedArrowType();
            int in,out,none;
            in = std::get<0>(result);
            out = std::get<1>(result);
            none =  std::get<2>(result);
            qDebug() << "进入菱形:" << in;
            qDebug() << "输出菱形:" << out;
            qDebug() << "无输入输出:" << none;
            if(in < 1) qDebug() << "输入不足1";
            if(out < 2) qDebug() << "输出不足2";
            qDebug() << none << "个连线未指定类型";
            continue;
        }
        if(dynamic_cast<DRectItem*>(item)){
            qDebug() << "找到一个矩形";
            DRectItem *rectItem = dynamic_cast<DRectItem*>(item);
            auto result = rectItem->getLinedArrowType();
            int in,out,none;
            in = std::get<0>(result);
            out = std::get<1>(result);
            none =  std::get<2>(result);
            qDebug() << "进入矩形:" << in;
            qDebug() << "输出矩形:" << out;
            qDebug() << "无输入输:" << none;
            if(in < 1) qDebug() << "输入不足1";
            if(out < 1) qDebug() << "输出不足1";
            qDebug() << none << "个连线未指定类型";
            continue;
        }
    }
}
