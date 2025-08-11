#pragma once

#include "TLayerGroup.hpp"
#include "FeatureCollection.h"
#include "IFeatureLayer.h"

class FeatureCollectionLayer: public TLayerGroup<IFeatureLayer>{
    Q_OBJECT

    public:
        FeatureCollectionLayer(FeatureCollection *collection,QObject *parent=nullptr);
        ~FeatureCollectionLayer();

        FeatureCollection *getFeature() override;

    private:
        FeatureCollection *collection;
};