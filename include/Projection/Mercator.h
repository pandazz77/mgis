#pragma once

#include "Projection.h"

class Mercator: public Projection{ // Used by the EPSG:3395 CRS
    public:
        Point2D project(LatLng latLng) override;
        LatLng unproject(Point2D pos) override;

        static constexpr double EARTH_RADIUS = 6378137;
        static constexpr double EARTH_RADIUS_MINOR = 6356752.314245179;

        Bounds<Point2D> bounds() override;
};