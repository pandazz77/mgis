#include "MapPane.h"

MapPane::MapPane(MapPane *parent): parent(parent){

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