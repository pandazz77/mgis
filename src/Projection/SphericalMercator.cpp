#include "SphericalMercator.h"

#define _USE_MATH_DEFINES
#include <math.h>

Point2D SphericalMercator::project(LatLng latLng){
    const double d = M_PI / 180,
        max = MAX_LATITUDE,
        lat = std::max(std::min(max,latLng.lat),-max),
        lsin = sin(lat * d);
    
    return Point2D(
        EARTH_RADIUS * latLng.lng * d,
        EARTH_RADIUS * log((1 + lsin) / (1 - lsin)) / 2
    );
}

LatLng SphericalMercator::unproject(Point2D pos){
    const double d = 180 / M_PI;

    return LatLng(
        (2 * atan(exp(pos.y / EARTH_RADIUS)) - (M_PI / 2)) * d,
        pos.x * d / EARTH_RADIUS
    );
}

Bounds<Point2D> SphericalMercator::bounds(){
    const double d = EARTH_RADIUS * M_PI;
    return Bounds<Point2D>{
        {d,d},
        {-d,-d}
    };
}

// Point2D SphericalMercator::project(LatLng latLng){
//     Point2D result;
//     result.x = (latLng.lng * 20037508.34) / 180;
//     result.y = log(tan((90+latLng.lat)*M_PI /360)) / (M_PI / 180);
//     result.y = (result.y * 20037508.34) / 180;
//     return result;
// }

// LatLng SphericalMercator::unproject(Point2D pos){
//     LatLng result;
//     result.lng = (pos.x * 180) / 20037508.34;
//     result.lat = (pos.y * 180) / 20037508.34;
//     result.lat = (atan(pow(M_E,result.lat*(M_PI/180))) * 360) / M_PI - 90;
//     return result;
// }