#include "saveandloadmanager.h"
#include "serializer.h"

#include <QFile>

void SaveAndLoadManager::bindScene(DScene *scene){
    this -> scene = scene;
}

bool SaveAndLoadManager::saveToFile(const QString &path){
    qDebug() << "save";
    if(scene != nullptr) saveToFile(path, scene);
    else return false;
    return true;
}

bool SaveAndLoadManager::saveToFile(const QString &path, DScene *scene){
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly)){
        qDebug() << "unable to open file" << file.errorString();
        return false;
    }

    QDataStream out(&file);
    out.setVersion(DConst::DATA_STREAM_VERSION);

    Serializer::instance().serializeSceneItems(out, scene);

    file.close();
    return true;
}

bool SaveAndLoadManager::loadFromFile(const QString &path, DScene *scene){
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "unable to open file" << file.errorString();
        return false;
    }

    QDataStream in(&file);
    in.setVersion(DConst::DATA_STREAM_VERSION);

	QList<QGraphicsItem*> data = Serializer::instance().deserializeItems(in);

    file.close();
    qDebug() << data.size();

    scene->clear();
    for(QGraphicsItem* item : data){
        if(item->parentItem() == nullptr) scene->addItem(item);
    }

    return true;
}

bool SaveAndLoadManager::loadFromFile(const QString &path){
    if(scene != nullptr) loadFromFile(path, scene);
    else return false;
    return true;
}
