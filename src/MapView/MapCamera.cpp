#include "MapCamera.h"

#include "MapGraphicsView.h"

MapCamera::MapCamera(LatLng pos, double scale, MapGraphicsView *map) : QObject(map), map(map), pos(pos), scale(scale){

}

MapCamera::~MapCamera(){

}


Point2D MapCamera::getProjectedPos(){
    return map->getProjection()->project(pos);
}

LatLng MapCamera::getPos(){
    return pos;
}

double MapCamera::getScale(){
    return scale;
}

void MapCamera::setPos(LatLng pos){
    this->pos = pos;
    emit posChanged(this->pos);
    emit projectedPosChanged(this->map->getProjection()->project(pos));
}

void MapCamera::setProjectedPos(Point2D pos){
    setPos(map->getProjection()->unproject(pos));
}

void MapCamera::setScale(double scale){
    this->scale = scale;
    emit scaleChanged(this->scale);
}

void MapCamera::zoomBy(double zoom){
    setScale(this->scale*zoom);
}