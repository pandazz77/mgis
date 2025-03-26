#pragma once

#include "Projection.h"

class SphericalMercator: public Projection{ // Used by the EPSG:3857 CRS
    public:
        Point2D project(LatLng latLng) override;
        LatLng unproject(Point2D pos) override;

        static constexpr double EARTH_RADIUS = 6378137;
        static constexpr double MAX_LATITUDE = 85.0511287798;

        Bounds<Point2D> bounds() override;
};