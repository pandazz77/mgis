#include "ILayer.h"

#include "MapGraphicsView.h"

ILayer::ILayer(QObject *parent): QObject(parent){

}

void ILayer::addTo(MapPane *pane){
    // STRONG ORDER / DO NOT CHANGE
    this->pane = pane;
    rebuildItem();
    if(pane->map()) pane->map()->addLayer(this);
}

void ILayer::removeFrom(MapPane *pane){
    // STRONG ORDER / DO NOT CHANGE
    if(pane->map()) pane->map()->removeLayer(this);
    this->pane = nullptr;
    rebuildItem();
}