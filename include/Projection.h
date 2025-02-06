#pragma once

#include "GeoTypes.hpp"

class Projection{
    public:
        virtual Point2D project(LatLng latLng) = 0;
        virtual LatLng unproject(Point2D pos) = 0;

        Point2D transform(LatLng pos);
        LatLng transform(Point2D pos);

        // void crop(Point2D pos); // TODO: crop pos/geometry by projection bounds

        template<class SourceUnit,class TargetUnit>
        void transform(const Geometry<SourceUnit> &sourceGeometry, Geometry<TargetUnit> &targetGeometry, bool project);

        void project(const Geometry<LatLng> &geometry, Geometry<Point2D> &projected);
        void unproject(const Geometry<Point2D> &geometry, Geometry<LatLng> &unprojected);

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