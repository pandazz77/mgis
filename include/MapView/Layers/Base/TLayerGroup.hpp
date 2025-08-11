#pragma once

#include "LayerSet.hpp"
#include "ILayer.h"


/**
 * @brief Base template class that implements a layer group.
 * 
 * @tparam TLayerPrime Prime layer class that will be used as a foundation
 * @tparam TLayerUnit  Layer-unit stored in a group
 */
template <typename TLayerPrime,typename TLayerUnit>
class TLayerGroupBase: public TLayerPrime, public LayerSet<TLayerUnit>{
    public:
        TLayerGroupBase(QObject *parent=nullptr): TLayerPrime(parent){}
        TLayerGroupBase(std::initializer_list<TLayerUnit*> layers, QObject *parent=nullptr): TLayerGroupBase<TLayerPrime,TLayerUnit>(parent){
            for(auto layer: layers) LayerSet<TLayerUnit>::addLayer(layer);
        }
        ~TLayerGroupBase(){
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

/**
 * @brief Simple template layer group class with ILayer as a prime 
 * 
 * @tparam TLayer Layer-unit stored in a group
 */
template<typename TLayer>
using TLayerGroup = TLayerGroupBase<ILayer,TLayer>;

/**
 * @brief United template layer group class, where prime and unit classes are equal
 * 
 * @tparam TLayer Prime/Unit class
 */
template<typename TLayer>
using TLayerGroupUnited = TLayerGroupBase<TLayer,TLayer>;

/**
 * @brief Splitted template layer group class, where prime and unit classes are different
 * 
 * @tparam TLayerPrime Prime layer class that will be used as a foundation
 * @tparam TLayerUnit Layer-unit stored in a group
 */
template<typename TLayerPrime,typename TLayerUnit>
using TLayerGroupSplitted = TLayerGroupBase<TLayerPrime,TLayerUnit>;