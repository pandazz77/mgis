#include "MapGraphicsScene.h"

#include "QFeatureConvertor.h"

MapGraphicsScene::MapGraphicsScene(QObject *parent) : QGraphicsScene(parent){

}

MapGraphicsScene::~MapGraphicsScene(){

}

void MapGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
    emit mouseMoved(QFeatureConvertor::qPoint(mouseEvent->scenePos()));
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void MapGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if(mouseEvent->lastScreenPos()==mouseEvent->screenPos())
        emit clicked(QFeatureConvertor::qPoint(mouseEvent->scenePos()));
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void MapGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if(mouseEvent->lastScreenPos()==mouseEvent->screenPos())
        emit doubleClicked(QFeatureConvertor::qPoint(mouseEvent->scenePos()));
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}
