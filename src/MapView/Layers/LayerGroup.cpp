#include "LayerGroup.h"

LayerGroup::LayerGroup(std::initializer_list<ILayer *> layers, QObject *parent): ILayer(parent){
    for(ILayer *l: layers){
        addLayer(l);
    }
}

LayerGroup::~LayerGroup(){
    for(ILayer *l: layers){
        l->deleteLater();
    }
}

QSet<ILayer*> LayerGroup::getLayers(){
    return layers;
}


QGraphicsItem *LayerGroup::getItem(){
    return group;
}

void LayerGroup::rebuildItem(MapPane *pane){
    if(group) delete group;

    group = new QGraphicsItemGroup;
    for(ILayer *l: layers){
        l->rebuildItem(pane);
        group->addToGroup(l->getItem());
    }
}