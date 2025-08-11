#pragma once

#include "LayerSet.hpp"
#include "ILayer.h"

template <typename TLayer>
class TLayerGroup: public TLayer, public LayerSet<TLayer>{
    public:
        TLayerGroup(QObject *parent=nullptr): TLayer(parent){}
        TLayerGroup(std::initializer_list<TLayer*> layers, QObject *parent=nullptr): TLayerGroup<TLayer>(parent){
            for(auto layer: layers) LayerSet<TLayer>::addLayer(layer);
        }
        ~TLayerGroup(){
            for(ILayer *l: this->getLayers()) l->deleteLater();
        }

        QGraphicsItem *getItem() override{
            return group;
        }

    protected:
        void rebuildItem(MapPane *pane) override{
            if(group) delete group;

            group = new QGraphicsItemGroup;
            for(ILayer *l: this->getLayers()){
                l->rebuildItem(pane);
                group->addToGroup(l->getItem());
            }
        }
        
    private:
        QGraphicsItemGroup *group = nullptr;
};