#include "SimpleStyleProvider.h"

SimpleStyleProvider::SimpleStyleProvider(){

}

SimpleStyleProvider::SimpleStyleProvider(std::function<sig> callback){
    setCallback(callback);
}

void SimpleStyleProvider::setCallback(std::function<sig> callback){
    this->onNewFeatureCallback = callback;
    /// TODO: APPLY NEW STYLER
}

void SimpleStyleProvider::removeCallback(){
    this->onNewFeatureCallback = nullptr;
    /// TODO: APPLY NEW STYLER
}

void SimpleStyleProvider::onNewFeature(const Feature *feature, IStyler *& styler){
    if(onNewFeatureCallback) return onNewFeatureCallback(feature,styler);
}
