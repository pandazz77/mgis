#pragma once

#include "LayerProvider.hpp"
#include "IFeatureLayer.h"

class FeatureLayerProvider: public LayerProvider<IFeatureLayer>{
    public:
        FeatureLayerProvider(QObject *parent=nullptr);

        // IFeatureLayer factory
        static IFeatureLayer *createFeatureLayer(IFeature* feature);
};