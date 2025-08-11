#pragma once

#include "LayerSet.hpp"
#include "ILayer.h"

template <typename T>
class TLayerGroup: public T, public LayerSet<T>{
    public:
        TLayerGroup(QObject *parent=nullptr): T(parent){}
        TLayerGroup(std::initializer_list<T*> layers, QObject *parent=nullptr): TLayerGroup<T>(parent){
            for(auto layer: layers) LayerSet<T>::addLayer(layer);
        }
        ~TLayerGroup(){
            for(ILayer *l: LayerSet<T>::layers) l->deleteLater();
        }

        QGraphicsItem *getItem() override{
            return group;
        }

    protected:
        void rebuildItem(MapPane *pane) override{
            if(group) delete group;

            group = new QGraphicsItemGroup;
            for(ILayer *l: LayerSet<T>::layers){
                l->rebuildItem(pane);
                group->addToGroup(l->getItem());
            }
        }
        
    private:
        QGraphicsItemGroup *group = nullptr;
};