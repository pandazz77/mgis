#pragma once

#include "IFeature.h"
#include "ILayer.h"

class IFeatureLayer: public ILayer{
    public:
        IFeatureLayer(QObject *parent = nullptr);

        virtual IFeature *getFeature() = 0;
};