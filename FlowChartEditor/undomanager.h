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

    void trimStack();

    void undo();
    void redo();

    void clearStack();

private:
    UndoManager() = default;
    UndoManager(const UndoManager&) = delete;
    UndoManager& operator=(const UndoManager&) = delete;

    QStack<QByteArray> undoStack;
    QStack<QByteArray> redoStack;

    DScene *scene = nullptr;

    void printStackSize();
};

#endif // UNDOMANAGER_H
