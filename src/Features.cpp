#include "Features.h"

Feature::Feature(Geometry<LatLng> *geometry): geometry(geometry){

}

Feature::~Feature(){
    delete geometry;
}

FeatureCollection::FeatureCollection(std::vector<IFeature*> features): std::vector<IFeature*>(features){

}

FeatureCollection::~FeatureCollection(){
    for(int i = 0; i < size(); i++){
        delete at(i);
    }
}