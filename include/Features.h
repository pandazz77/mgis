#pragma once

#include <unordered_map>
#include <string>
#include <variant>

#include "GeoTypes.hpp"
#include "Stylers.h"

class IFeature{
    public:
        IFeature(){};
        virtual ~IFeature(){}; 
};

class Feature : public IFeature{
    public:
        Feature(Geometry<LatLng> *geometry);
        ~Feature();

        Geometry<LatLng> *geometry;
        IStyler *styler = RandomStyler::getInstance(); /// TODO: styler field as shared ptr 
        std::unordered_map<std::string,std::variant<int,double,bool,std::string>> properties;
};

class FeatureCollection: public std::vector<IFeature*>, IFeature{
    public:
        FeatureCollection(std::vector<IFeature*> features = {});
        ~FeatureCollection();
};