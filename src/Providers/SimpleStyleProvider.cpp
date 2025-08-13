#include "SimpleStyleProvider.h"

SimpleStyleProvider::SimpleStyleProvider(){

}

SimpleStyleProvider::SimpleStyleProvider(std::function<IStyler*(Feature*)> callback){
    setCallback(callback);
}

void SimpleStyleProvider::setCallback(std::function<IStyler*(Feature*)> callback){
    this->onNewFeatureCallback = callback;
    /// TODO: APPLY NEW STYLER
}

void SimpleStyleProvider::removeCallback(){
    this->onNewFeatureCallback = nullptr;
    /// TODO: APPLY NEW STYLER
}

IStyler *SimpleStyleProvider::onNewFeature(Feature *feature){
    if(onNewFeatureCallback) return onNewFeatureCallback(feature);
    return nullptr;
}
