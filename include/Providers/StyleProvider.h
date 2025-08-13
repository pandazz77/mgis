#pragma once

#include "Stylers.h"
#include "Feature.h"

class StyleProvider{
    public:
        StyleProvider();

    protected:
        friend class FeatureLayerProvider;
        virtual IStyler *onNewFeature(Feature *feature) = 0;
};