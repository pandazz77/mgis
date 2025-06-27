#include "LayerGroup.h"

LayerGroup::LayerGroup(std::initializer_list<ILayer *> layers, QObject *parent): ILayer(parent), layers(layers){

}

LayerGroup::~LayerGroup(){
    for(ILayer *l: layers){
        l->deleteLater();
    }
}

void LayerGroup::addLayer(ILayer *layer){
    layers.insert(layer);
}

void LayerGroup::removeLayer(ILayer *layer){
    layers.remove(layer);
}

QSet<ILayer*> LayerGroup::getLayers(){
    return layers;
}


QGraphicsItem *LayerGroup::getItem(){
    return group;
}

void LayerGroup::rebuildItem(){
    if(group) delete group;

    group = new QGraphicsItemGroup;
    for(ILayer *l: layers){
        l->rebuildItem();
    }
}