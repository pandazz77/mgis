#include "Mercator.h"

#define _USE_MATH_DEFINES
#include <math.h>

Point2D Mercator::project(LatLng latLng){
    Point2D result;
    const double d = M_PI / 180,
        r = EARTH_RADIUS,
        tmp = EARTH_RADIUS_MINOR / r,
        e = sqrt(1 - tmp * tmp);
    result.y = latLng.lat * d;
    const double con = e * sin(result.y);
    const double ts = tan(M_PI/4-result.y/2) / pow((1-con)/(1+con),e/2);
    result.y = -r * log(std::max(ts,1E-10));
    result.x = latLng.lng * d * r;
    return result;
}

LatLng Mercator::unproject(Point2D pos){
    LatLng result;
    const double d = 180 / M_PI,
        r = EARTH_RADIUS,
        tmp = EARTH_RADIUS_MINOR / r,
        e = sqrt(1 - tmp * tmp),
        ts = exp(-pos.y / r);
    double phi = M_PI / 2 - 2 * atan(ts);

    for(double i = 0, dphi = 0.1, con; i < 15 && abs(dphi) > 1e-7; i++){
        con = e * sin(phi);
        con = pow((1 - con) / (1 + con), e / 2);
        dphi = M_PI / 2 - 2 * atan(ts * con) - phi;
        phi += dphi;
    }

    result.lat = phi * d;
    result.lng = pos.x * d / r;
    return result;
}

Bounds<Point2D> Mercator::bounds(){
    return Bounds<Point2D>{
        {20037508.34279, 18764656.23138},
        {-20037508.34279, -15496570.73972}
    };
}