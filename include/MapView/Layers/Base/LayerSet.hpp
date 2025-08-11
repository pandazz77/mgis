#pragma once

#include <QSet>

template<typename TLayer>
class LayerSet{
    public:
        virtual void addLayer(TLayer *layer){ 
            layers.insert(layer);
        }
        virtual void removeLayer(TLayer *layer){
            layers.remove(layer);
        }
        QSet<TLayer*> getLayers(){
            return layers;
        }

    protected:
        QSet<TLayer *> layers;
};