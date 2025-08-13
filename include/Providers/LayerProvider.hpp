#pragma once

#include <TLayerGroup.hpp>

template <typename TLayer>
class LayerProvider: public TLayerGroup<TLayer>{
    public:
        LayerProvider(QObject *parent=nullptr) : TLayerGroup<TLayer>(parent){
            
        }
};