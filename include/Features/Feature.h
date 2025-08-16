#pragma once

#include "IFeature.h"
#include "GeoTypes.hpp"

#include "FeatureProperties.hpp"

class Feature : public IFeature{
    public:
        using Properties = FeatureProperties;

        Feature(Geometry *geometry);
        ~Feature();

        Geometry *geometry;
        Properties properties;
};