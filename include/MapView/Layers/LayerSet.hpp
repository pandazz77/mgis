#pragma once

#include <QSet>

template<typename T>
class LayerSet{
    public:
        virtual void addLayer(T *layer){ 
            layers.insert(layer);
        }
        virtual void removeLayer(T *layer){
            layers.remove(layer);
        }
        QSet<T*> getLayers(){
            return layers;
        }

    protected:
        QSet<T *> layers;
};