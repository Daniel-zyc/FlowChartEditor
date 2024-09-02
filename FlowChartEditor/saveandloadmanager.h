#ifndef SAVEANDLOADMANAGER_H
#define SAVEANDLOADMANAGER_H
#include <QString>

#include "dscene.h"

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
    bool saveToFile(const QString &fileName, DScene *scene);

    bool loadFromFile(const QString &fileName);
    bool loadFromFile(const QString &fileName, DScene *scene);

    void bindScene(DScene *scene);

private:
    SaveAndLoadManager() = default;

    DScene *scene;
};

#endif // SAVEANDLOADMANAGER_H
