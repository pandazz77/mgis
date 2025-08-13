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