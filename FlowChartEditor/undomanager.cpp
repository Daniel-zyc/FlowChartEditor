#include "undomanager.h"
#include "serializer.h"

UndoManager::UndoManager(){
    QByteArray data; QDataStream out(&data,QIODevice::WriteOnly);
    Serializer::instance().serializeEmptyItems(out);
    undoStack.push(data);
}

void UndoManager::shot(QGraphicsScene *scene){
    qDebug() << "shot";
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    QList<QGraphicsItem *> items = scene->items();
    Serializer::instance().serializeSceneItems(out, items);
    if(undoStack.empty() || data != undoStack.top()) undoStack.push(data);
    trimStack();
    SHOT_STATE = DConst::UNCHANGED;
}

void UndoManager::shot(){
    if(scene == nullptr) return;
    shot(scene);
}

void UndoManager::undo(){
    // if(!undoStack.empty()) redoStack.push(undoStack.pop());    // 当前快照压入
    printStackSize();
    if(scene == nullptr || undoStack.empty()) return;
    scene->clear();

    QByteArray data = undoStack.pop();
    QDataStream in(&data,QIODevice::ReadOnly);
    QList<QGraphicsItem *> items = Serializer::instance().deserializeSceneItems(in);
    for(QGraphicsItem * item : items){
        if(item->parentItem() == nullptr) scene->addItem(item);
    }
    redoStack.push(data);
    trimStack();
}

void UndoManager::redo(){
    // if(!redoStack.empty()) undoStack.push(redoStack.pop());
    printStackSize();
    if(scene == nullptr || redoStack.empty()) return;
    scene->clear();

    QByteArray data = redoStack.pop();
    QDataStream in(&data,QIODevice::ReadOnly);
    QList<QGraphicsItem *> items = Serializer::instance().deserializeSceneItems(in);
    for(QGraphicsItem * item : items){
        if(item->parentItem() == nullptr) scene->addItem(item);
    }
    undoStack.push(data);
    trimStack();
}

void UndoManager::trimStack(){
    while(undoStack.size() > DConst::MAX_UNDO_STACK_SIZE) undoStack.removeFirst();
    while(redoStack.size() > DConst::MAX_REDO_STACK_SIZE) redoStack.removeFirst();
}

void UndoManager::bindScene(DScene *scene){
    this -> scene = scene;
}

void UndoManager::clearStack(){
    undoStack.clear();
    redoStack.clear();
}

void UndoManager::printStackSize(){
    qDebug() << "redoStack size"<< redoStack.size()
             << "undoStack size"<< undoStack.size();
}
