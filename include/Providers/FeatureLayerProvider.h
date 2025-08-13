#pragma once

#include "LayerProvider.hpp"
#include "IFeatureLayer.h"
#include "StyleProvider.h"
#include "FeatureLayer.h"

class FeatureLayerProvider: public LayerProvider<IFeatureLayer>{
    public:
        FeatureLayerProvider(QObject *parent=nullptr);

        virtual void addLayer(IFeatureLayer *layer) override;

        void setStyleProvider(StyleProvider *styleProvider);
        StyleProvider *getStyleProvider();

        // IFeatureLayer factory
        static IFeatureLayer *createFeatureLayer(IFeature* feature);

    private:
        void processLayerWithStyleProvider(FeatureLayer *layer);

    private:
        StyleProvider *styleProvider = nullptr;
};