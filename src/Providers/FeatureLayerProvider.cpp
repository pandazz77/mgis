#include "FeatureLayerProvider.h"
#include "Feature.h"
#include "FeatureCollection.h"
#include "FeatureLayer.h"
#include "FeatureCollectionLayer.h"

FeatureLayerProvider::FeatureLayerProvider(QObject *parent) : LayerProvider<IFeatureLayer>(parent){

}

IFeatureLayer *FeatureLayerProvider::createFeatureLayer(IFeature* feature){
    if(dynamic_cast<Feature*>(feature)) 
        return new FeatureLayer(dynamic_cast<Feature*>(feature));
    else if(dynamic_cast<FeatureCollection*>(feature)) 
        return new FeatureCollectionLayer(dynamic_cast<FeatureCollection*>(feature));
    return nullptr;
}

/// TODO: Move to somewhere else and make public
void onEachFeature(const FeatureCollection &collection,const std::function<void(Feature*)> &callback, bool deep=true){
    for(IFeature* feature: collection){
        if(dynamic_cast<Feature*>(feature)) 
            callback(dynamic_cast<Feature*>(feature));
        else if(dynamic_cast<FeatureCollection*>(feature))
            if(deep) onEachFeature(*dynamic_cast<FeatureCollection*>(feature),callback,deep);
    }
}

/// TODO: Move to somewhere else and make public
void onEachFeatureLayer(FeatureCollectionLayer *collection, const std::function<void(FeatureLayer*)> &callback, bool deep=true){
    for(IFeatureLayer* feature: collection->getLayers()){
        if(dynamic_cast<FeatureLayer*>(feature)) 
            callback(dynamic_cast<FeatureLayer*>(feature));
        else if(dynamic_cast<FeatureCollection*>(feature))
            if(deep) onEachFeatureLayer(dynamic_cast<FeatureCollectionLayer*>(feature),callback,deep);
    }
}

void FeatureLayerProvider::addLayer(IFeatureLayer *layer){
    if(styleProvider){
        if(dynamic_cast<FeatureLayer*>(layer)){
            processLayerWithStyleProvider(dynamic_cast<FeatureLayer*>(layer));
        } else if(dynamic_cast<FeatureCollectionLayer*>(layer)){
            onEachFeatureLayer(dynamic_cast<FeatureCollectionLayer*>(layer),[this](FeatureLayer *flayer){
                this->processLayerWithStyleProvider(flayer);
            });
        }
    }


    LayerProvider<IFeatureLayer>::addLayer(layer);
}

void FeatureLayerProvider::setStyleProvider(StyleProvider *styleProvider){
    this->styleProvider = styleProvider;
}

StyleProvider *FeatureLayerProvider::getStyleProvider(){
    return styleProvider;
}

void FeatureLayerProvider::processLayerWithStyleProvider(FeatureLayer *layer){
    styleProvider->onNewFeature(layer->getFeature(),layer->styler);
}
