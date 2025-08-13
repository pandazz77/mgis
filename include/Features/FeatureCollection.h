#pragma once

#include "IFeature.h"
#include <vector>

class FeatureCollection: public std::vector<IFeature*>, public IFeature{
    public:
        FeatureCollection(std::vector<IFeature*> features = {});
        ~FeatureCollection();
};