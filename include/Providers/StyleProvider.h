#pragma once

#include "FeatureLayer.h"

class StyleProvider{
    public:
        StyleProvider();

    protected:
        friend class FeatureLayerProvider;
        virtual void onNewFeature(FeatureLayer *feature) = 0;
};