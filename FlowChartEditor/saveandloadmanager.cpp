#include "saveandloadmanager.h"

#include <QFile>

void SaveAndLoadManager::bindScene(QGraphicsScene *scene){
    this -> scene = scene;
}

bool SaveAndLoadManager::saveToFile(const QString &path){
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly)){
        qDebug() << "unable to open file" << file.errorString();
        return false;
    }

    QDataStream out(&file);
    out.setVersion(DataStreamVersion);

    Serializer::instance().serializeSceneItems(out, scene);

    file.close();

    return true;
}

bool SaveAndLoadManager::saveToFile(const QString &path, QGraphicsScene *scene){
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly)){
        qDebug() << "unable to open file" << file.errorString();
        return false;
    }

    QDataStream out(&file);
    out.setVersion(DataStreamVersion);

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
    in.setVersion(DataStreamVersion);

    Serializer::instance().deserializeSceneItems(in, scene);

    file.close();

    return true;
}

bool SaveAndLoadManager::loadFromFile(const QString &path){
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "unable to open file" << file.errorString();
        return false;
    }

    QDataStream in(&file);
    in.setVersion(DataStreamVersion);

    Serializer::instance().deserializeSceneItems(in, scene);

    file.close();

    return true;
}
