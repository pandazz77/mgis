#include "SimpleStyleProvider.h"

SimpleStyleProvider::SimpleStyleProvider(){

}

SimpleStyleProvider::SimpleStyleProvider(std::function<void(FeatureLayer*)> callback){
    setCallback(callback);
}

void SimpleStyleProvider::setCallback(std::function<void(FeatureLayer*)> callback){
    this->onNewFeatureCallback = callback;
    /// TODO: APPLY NEW STYLER
}

void SimpleStyleProvider::removeCallback(){
    this->onNewFeatureCallback = nullptr;
    /// TODO: APPLY NEW STYLER
}

void SimpleStyleProvider::onNewFeature(FeatureLayer *feature){
    if(onNewFeatureCallback) return onNewFeatureCallback(feature);
}
