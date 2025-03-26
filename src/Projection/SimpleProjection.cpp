#include "SimpleProjection.h"

Point2D SimpleProjection::project(LatLng latLng){
    return Point2D(latLng.lng,latLng.lat);
}

LatLng SimpleProjection::unproject(Point2D pos){
    return LatLng(pos.y,pos.x);
}

Bounds<Point2D> SimpleProjection::bounds(){
    return Bounds<Point2D>{
        {180,90},
        {-180,-90}
    };
}