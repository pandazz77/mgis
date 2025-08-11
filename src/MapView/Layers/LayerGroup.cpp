#include "LayerGroup.h"

LayerGroup::LayerGroup(QObject *parent): TLayerGroup<ILayer>(parent){

}

LayerGroup::LayerGroup(std::initializer_list<ILayer*> layers, QObject *parent): TLayerGroup<ILayer>(layers,parent){

}

LayerGroup::~LayerGroup(){

}
