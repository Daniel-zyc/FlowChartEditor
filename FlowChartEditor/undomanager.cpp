#include "undomanager.h"
#include "serializer.h"

UndoManager::UndoManager(){
    QByteArray data; QDataStream out(&data,QIODevice::WriteOnly);
    Serializer::instance().serializeEmptyItems(out);
    undoStack.push(data);
}

void UndoManager::shot(QGraphicsScene *scene){
	// qDebug() << "shot";
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    QList<QGraphicsItem *> items = scene->items();
	Serializer::instance().serializeItems(out, items);
    if(undoStack.empty() || data != undoStack.top()) undoStack.push(data);
    trimStack();
    SHOT_STATE = DConst::UNCHANGED;
	// qDebug() << "shot finish";
}

void UndoManager::shot(){
    if(scene == nullptr) return;
    shot(scene);
}

void UndoManager::undo(){

	// printStackSize();
    if(scene == nullptr || undoStack.empty()) return;
    if(undoStack.size() > 1) redoStack.push(undoStack.pop());
    if(undoStack.empty()) return;
    QByteArray data = undoStack.top();
    QDataStream in(&data,QIODevice::ReadOnly);
    QList<QGraphicsItem *> items = Serializer::instance().deserializeItems(in);
    scene->clear(); scene->dDrawItems(items);
    trimStack();
}

void UndoManager::redo(){
	// printStackSize();
    if(scene == nullptr || redoStack.empty()) return;
    QByteArray data = redoStack.pop();
    QDataStream in(&data,QIODevice::ReadOnly);
    QList<QGraphicsItem *> items = Serializer::instance().deserializeItems(in);
    scene->clear(); scene->dDrawItems(items);
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
