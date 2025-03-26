#include "Projection.h"

Point2D Projection::transform(LatLng pos){
    return project(pos);
}

LatLng Projection::transform(Point2D pos){
    return unproject(pos);
}