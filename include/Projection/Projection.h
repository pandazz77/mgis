#pragma once

#include "GeoTypes.hpp"

class Projection{
    public:
        virtual Point2D project(LatLng latLng) = 0;
        virtual LatLng unproject(Point2D pos) = 0;

        Point2D transform(LatLng pos);
        LatLng transform(Point2D pos);

        // void crop(Point2D pos); // TODO: crop pos/geometry by projection bounds

        template<typename SourceUnit, typename TargetUnit>
        Bounds<TargetUnit> transform(const Bounds<SourceUnit> &source){
            return Bounds<TargetUnit>(
                transform(source.NE),
                transform(source.SW)
            );
        }

        template<typename SourceUnit, typename TargetUnit>
        Point<TargetUnit> transform(const Point<SourceUnit> &source){
            return Point<TargetUnit>(transform(source.coordinates));
        }

        template<typename SourceUnit, typename TargetUnit>
        LineString<TargetUnit> transform(const LineString<SourceUnit> &source){
            LineString<TargetUnit> result;
            
            for(SourceUnit sUnit: source){
                result.push_back(transform(sUnit));
            }
            return result;
        }

        template<typename SourceUnit, typename TargetUnit>
        Polygon<TargetUnit> transform(const Polygon<SourceUnit> &source){
            Polygon<TargetUnit> result;

            result.exterior = transform<SourceUnit,TargetUnit>(source.exterior);
            
            for(LinearRing<SourceUnit> sInterior: source.interiors){
                result.interiors.push_back(transform<SourceUnit,TargetUnit>(sInterior));
            }

            return result;
        }

        template<template<typename> class Collection, typename SourceUnit, typename TargetUnit>
        Collection<TargetUnit> transformCollection(const Collection<SourceUnit>& source) {
            Collection<TargetUnit> result;

            for (const auto& sGeometry : source)
                result.push_back(transform<SourceUnit, TargetUnit>(sGeometry));

            return result;
        }

        template<typename SourceUnit, typename TargetUnit>
        MultiPoint<TargetUnit> transform(const MultiPoint<SourceUnit> &source){
            return transformCollection<MultiPoint, SourceUnit, TargetUnit>(source);
        }

        template<typename SourceUnit, typename TargetUnit>
        MultiLineString<TargetUnit> transform(const MultiLineString<SourceUnit> &source){
            return transformCollection<MultiLineString, SourceUnit, TargetUnit>(source);
        }
        
        template<typename SourceUnit, typename TargetUnit>
        MutliPolygon<TargetUnit> transform(const MutliPolygon<SourceUnit> &source){
            return transformCollection<MutliPolygon, SourceUnit, TargetUnit>(source);
        }

        template<template<typename> typename T>
        T<Point2D> project(T<LatLng> geometry){ return transform<LatLng,Point2D>(geometry); }
        template<template<typename> typename T>
        T<LatLng> unproject(T<Point2D> geometry){ return transform<Point2D,LatLng>(geometry); }

        virtual Bounds<Point2D> bounds() = 0;
};