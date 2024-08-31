#ifndef SAVEANDLOADMANAGER_H
#define SAVEANDLOADMANAGER_H

#include <QString>
#include "serializer.h"

class SaveAndLoadManager
{
public:
    static SaveAndLoadManager& instance(){
        static SaveAndLoadManager instance;
        return instance;
    }

    SaveAndLoadManager(const SaveAndLoadManager&) = delete;
    SaveAndLoadManager& operator=(const SaveAndLoadManager&) = delete;

    bool saveToFile(const QString &fileName);
    bool saveToFile(const QString &fileName, QGraphicsScene *scene);

    bool loadFromFile(const QString &fileName);
    bool loadFromFile(const QString &fileName, QGraphicsScene *scene);

    void bindScene(QGraphicsScene *scene);

private:
    SaveAndLoadManager() = default;

    QGraphicsScene *scene;
};

#endif // SAVEANDLOADMANAGER_H
