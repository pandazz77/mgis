#include "MapPane.h"

MapPane::MapPane(MapPane *parent): parent(parent){

}

void MapPane::addLayer(ILayer *layer){
    layers.insert(layer);
}

void MapPane::removeLayer(ILayer *layer){
    layers.remove(layer);
}

QSet<ILayer*> MapPane::getLayers(){
    return layers;
}

MapPane *MapPane::parentPane(){
    return parent;
}

MapGraphicsView *MapPane::map(){
    if(parent) return parent->map();
    return nullptr;
}

void MapPane::setParentPane(MapPane *parent){
    this->parent = parent;
}