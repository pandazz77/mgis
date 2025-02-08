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

        #define _transformCollection(Collection)                                \
        template<typename SourceUnit, typename TargetUnit>                      \
        Collection<TargetUnit> transform(const Collection<SourceUnit> &source){ \
            Collection<TargetUnit> result;                                      \
                                                                                \
            for(auto sGeometry: source)                                         \
                result.push_back(transform<SourceUnit,TargetUnit>(sGeometry));  \
                                                                                \
                                                                                \
            return result;                                                      \
        }                                                                       \

        _transformCollection(MultiPoint)
        _transformCollection(MultiLineString)
        _transformCollection(MutliPolygon)

        #define _projUnproj(Gtype) \
        Gtype<Point2D> project(Gtype<LatLng> geometry) { return transform<LatLng,Point2D>(geometry); } \
        Gtype<LatLng> unproject(Gtype<Point2D> geometry) { return transform<Point2D,LatLng>(geometry); } 

        _projUnproj(Bounds)
        _projUnproj(Point)
        _projUnproj(LineString)
        _projUnproj(Polygon)
        _projUnproj(MultiPoint)
        _projUnproj(MultiLineString)
        _projUnproj(MutliPolygon)

        virtual Bounds<Point2D> bounds() = 0;
};

class SimpleProjection: public Projection{ // Equirectangular projection / Used by the EPSG:4326 CRS
    public:
        Point2D project(LatLng latLng) override;
        LatLng unproject(Point2D pos) override;

        Bounds<Point2D> bounds() override;
};

class Mercator: public Projection{ // Used by the EPSG:3395 CRS
    public:
        Point2D project(LatLng latLng) override;
        LatLng unproject(Point2D pos) override;

        static constexpr double EARTH_RADIUS = 6378137;
        static constexpr double EARTH_RADIUS_MINOR = 6356752.314245179;

        Bounds<Point2D> bounds() override;
};

class SphericalMercator: public Projection{ // Used by the EPSG:3857 CRS
    public:
        Point2D project(LatLng latLng) override;
        LatLng unproject(Point2D pos) override;

        static constexpr double EARTH_RADIUS = 6378137;
        static constexpr double MAX_LATITUDE = 85.0511287798;

        Bounds<Point2D> bounds() override;
};