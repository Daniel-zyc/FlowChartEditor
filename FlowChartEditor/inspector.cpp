#include "inspector.h"
#include "ddiaitem.h"
#include "drectitem.h"

#include <QCloseEvent>

Inspector::Inspector(QWidget *parent,DScene* scene, DView *view)
    :QDialog(parent),scene(scene), view(view) {
    gridLayout->addWidget(message,0,0);
    gridLayout->addWidget(forwardBtn,1,0);
    gridLayout->addWidget(backwardBtn,1,1);
    setLayout(gridLayout);
    connect(backwardBtn,&QPushButton::clicked,this,&Inspector::backward);
    connect(forwardBtn,&QPushButton::clicked,this,&Inspector::forward);
}

// 菱形判定至少有一个输入两个输出
// 矩形判定至少有一个输入一个输出
// 平行四边形判定一个输入一个输出
void Inspector::check(){
    if(scene == nullptr || view == nullptr) return;
    originalCentrer = view->mapToScene(view->viewport()->rect().center());
    originalTransform = view->transform();
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
            if(in < 1) errorMessage.append({"判断输入小于1",item});
            if(out < 2) errorMessage.append({"判断输出小于2",item});
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
            if(in < 1) errorMessage.append({"输入小于1",item});
            if(out < 1) errorMessage.append({"输出小于2",item});
            qDebug() << none << "个连线未指定类型";
            continue;
        }
    }
    index = 0;
    if(errorMessage.empty()) return;
    view->centerOn(errorMessage.at(index).item);
    message->setText(errorMessage.at(index).message);
}

void Inspector::backward(){
    if(index < errorMessage.size() - 1) index ++;
    view->centerOn(errorMessage.at(index).item);
    message->setText(errorMessage.at(index).message);
}

void Inspector::forward(){
    if(index > 0) index --;
    view->centerOn(errorMessage.at(index).item);
    message->setText(errorMessage.at(index).message);
}

void Inspector::closeEvent(QCloseEvent *event) {
    errorMessage.clear();
    restoreView();
    event->accept();
}

void Inspector::restoreView(){
    if(view == nullptr) return;
    view->setTransform(originalTransform);
    view->centerOn(originalCentrer);
}
