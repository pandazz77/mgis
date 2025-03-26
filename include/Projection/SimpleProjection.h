#pragma once

#include "Projection.h"

class SimpleProjection: public Projection{ // Equirectangular projection / Used by the EPSG:4326 CRS
    public:
        Point2D project(LatLng latLng) override;
        LatLng unproject(Point2D pos) override;

        Bounds<Point2D> bounds() override;
};
