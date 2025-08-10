#pragma once

#include "IFeature.h"
#include "GeoTypes.hpp"

#include <unordered_map>
#include <string>
#include <variant>

class Feature : public IFeature{
    public:
        Feature(Geometry *geometry);
        ~Feature();

        Geometry *geometry;
        std::unordered_map<std::string,std::variant<int,double,bool,std::string>> properties;
};