#include "ILayer.h"

#include "MapGraphicsView.h"

ILayer::ILayer(QObject *parent): QObject(parent){

}

void ILayer::addTo(MapGraphicsView *map){
    // STRONG ORDER / DO NOT CHANGE
    this->map = map;
    onMapChanged();
    map->addLayer(this);
}

void ILayer::removeFrom(MapGraphicsView *map){
    // STRONG ORDER / DO NOT CHANGE
    map->removeLayer(this);
    this->map = nullptr;
    onMapChanged();
}