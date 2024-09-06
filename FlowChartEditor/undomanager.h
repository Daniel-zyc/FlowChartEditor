#ifndef UNDOMANAGER_H
#define UNDOMANAGER_H

#include <QStack>
#include <QTemporaryFile>
#include <QGraphicsItem>

#include "dscene.h"

class UndoManager
{
public:
    static UndoManager& instance(){
        static UndoManager instance;
        return instance;
    }

    void bindScene(DScene *scene);

    void shot();
    void shot(QGraphicsScene *scene);

    void undo();
    void redo();

private:
    UndoManager();
    UndoManager(const UndoManager&) = delete;
    UndoManager& operator=(const UndoManager&) = delete;

    void trimStack();

    void clearStack();

    QStack<QByteArray> undoStack;
    QStack<QByteArray> redoStack;

    DScene *scene = nullptr;

    void printStackSize();
};

#endif // UNDOMANAGER_H
