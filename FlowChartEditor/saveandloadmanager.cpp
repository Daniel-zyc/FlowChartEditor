#include "saveandloadmanager.h"

#include <QFile>

void SaveAndLoadManager::bindScene(QGraphicsScene *scene){
    this -> scene = scene;
}

bool SaveAndLoadManager::saveToFile(const QString &path){
    if(scene != nullptr) saveToFile(path, scene);
    else return false;
    return true;
}

bool SaveAndLoadManager::saveToFile(const QString &path, QGraphicsScene *scene){
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

bool SaveAndLoadManager::loadFromFile(const QString &path, QGraphicsScene *scene){
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "unable to open file" << file.errorString();
        return false;
    }

    QDataStream in(&file);
    in.setVersion(DConst::DATA_STREAM_VERSION);

    QList<QGraphicsItem*> data = Serializer::instance().deserializeSceneItems(in);

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

bool SaveAndLoadManager::copySelectedItems(QGraphicsScene *scene){
    if(temFile != nullptr) {
        temFile -> close();
        delete temFile;
        temFile = nullptr;
    }

    temFile = new QTemporaryFile;
    if(!temFile->open()){
        delete temFile;
        temFile = nullptr;
        return false;
    }

    QDataStream out(temFile);

    Serializer::instance().serializeSceneItems(out,scene->selectedItems());

    return temFile->flush();
    return true;
}

bool SaveAndLoadManager::copySelectedItems(){
    if(scene == nullptr) return false;
    copySelectedItems(scene);
    return true;
}

bool SaveAndLoadManager::pasteSelectedItems(QGraphicsScene *scene){
    if(!temFile || !temFile->isOpen()) return false;
    temFile->seek(0);
    QDataStream in(temFile);
    QList<QGraphicsItem *> items = Serializer::instance().deserializeSceneItems(in);
    DTool::moveItems(items);
    for(QGraphicsItem* item : items){
        if(item->parentItem() == nullptr) scene->addItem(item);
    }
    return true;
}

bool SaveAndLoadManager::pasteSeletedItems(){
    if(scene == nullptr) return false;
    pasteSelectedItems(scene);
    return true;
}
