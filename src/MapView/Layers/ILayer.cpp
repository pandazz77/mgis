#include "ILayer.h"

#include "MapGraphicsView.h"

ILayer::ILayer(QObject *parent): QObject(parent){

}

void ILayer::addTo(MapPane *pane){
    pane->addLayer(this);
}

void ILayer::removeFrom(MapPane *pane){
    pane->addLayer(this);
}