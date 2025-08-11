#pragma once

#include <QSet>

template<typename TLayer>
class LayerSet: private QSet<TLayer*>{
    using super = QSet<TLayer*>;

    public:
        virtual void addLayer(TLayer *layer){ 
            super::insert(layer);
        }
        virtual void removeLayer(TLayer *layer){
            super::remove(layer);
        }
        QSet<TLayer*> getLayers(){
            return *this;
        }
};