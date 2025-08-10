#include "FeatureCollectionLayer.h"
#include "FeatureLayer.h"


FeatureCollectionLayer::FeatureCollectionLayer(FeatureCollection *collection,QObject *parent) : LayerGroup(parent){
    for(IFeature* ifeature: *collection){
        if(dynamic_cast<Feature*>(ifeature)) 
            addLayer(new FeatureLayer(dynamic_cast<Feature*>(ifeature),this));
        else if(dynamic_cast<FeatureCollection*>(ifeature))
            addLayer(new FeatureCollectionLayer(dynamic_cast<FeatureCollection*>(ifeature),this));
    }
}

FeatureCollection *FeatureCollectionLayer::getCollection(){
    return collection;
}

FeatureCollectionLayer::~FeatureCollectionLayer(){

}
