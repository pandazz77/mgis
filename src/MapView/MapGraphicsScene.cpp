#include "MapGraphicsScene.h"


Point2D _mgisConvert(QPointF point){
    return Point2D(point.x(),point.y());
}

MapGraphicsScene::MapGraphicsScene(QObject *parent) : QGraphicsScene(parent){

}

MapGraphicsScene::~MapGraphicsScene(){

}

void MapGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
    emit mouseMoved(_mgisConvert(mouseEvent->scenePos()));
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void MapGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if(mouseEvent->lastScreenPos()==mouseEvent->screenPos())
        emit clicked(_mgisConvert(mouseEvent->scenePos()));
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void MapGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if(mouseEvent->lastScreenPos()==mouseEvent->screenPos())
        emit doubleClicked(_mgisConvert(mouseEvent->scenePos()));
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}
