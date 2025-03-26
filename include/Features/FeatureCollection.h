#pragma once

#include "IFeature.h"
#include <vector>

class FeatureCollection: public std::vector<IFeature*>, IFeature{
    public:
        FeatureCollection(std::vector<IFeature*> features = {});
        ~FeatureCollection();
};