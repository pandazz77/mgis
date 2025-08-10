#pragma once

#include "LayerGroup.h"
#include "FeatureCollection.h"

class FeatureCollectionLayer: public LayerGroup{
    Q_OBJECT

    public:
        FeatureCollectionLayer(FeatureCollection *collection,QObject *parent);
        ~FeatureCollectionLayer();

        FeatureCollection *getCollection();

    private:
        FeatureCollection *collection;
};