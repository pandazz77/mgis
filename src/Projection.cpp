#include "Projection.h"

#define _USE_MATH_DEFINES
#include <math.h>

Point2D Projection::transform(LatLng pos){
    return project(pos);
}

LatLng Projection::transform(Point2D pos){
    return unproject(pos);
}

#define IS_GEOMETRY(geom_ref,derived_geom_type) dynamic_cast<const derived_geom_type*>(&geom_ref)

template<class SourceUnit,class TargetUnit>
void Projection::transform(const Geometry<SourceUnit> &sourceGeometry, Geometry<TargetUnit> &targetGeometry, bool project){

    if(IS_GEOMETRY(sourceGeometry,Bounds<SourceUnit>)){
        Bounds<TargetUnit> &target = dynamic_cast<Bounds<TargetUnit>&>(targetGeometry);
        const Bounds<SourceUnit> &source = dynamic_cast<const Bounds<SourceUnit>&>(sourceGeometry);

        target.NE = transform(source.NE);
        target.SW = transform(source.SW);
    } else if(IS_GEOMETRY(sourceGeometry,Point<SourceUnit>)){
        Point<TargetUnit> &target = dynamic_cast<Point<TargetUnit>&>(targetGeometry);
        const Point<SourceUnit> &source = dynamic_cast<const Point<SourceUnit>&>(sourceGeometry);

        target.coordinates = transform(source.coordinates);
    } else if(IS_GEOMETRY(sourceGeometry,LineString<SourceUnit>)){
        LineString<TargetUnit> &target = dynamic_cast<LineString<TargetUnit>&>(targetGeometry);
        const LineString<SourceUnit> &source = dynamic_cast<const LineString<SourceUnit>&>(sourceGeometry);
        
        for(SourceUnit sUnit: source.coordinates){
            target.coordinates.push_back(transform(sUnit));
        }
    } else if(IS_GEOMETRY(sourceGeometry,Polygon<SourceUnit>)){
        Polygon<TargetUnit> &target = dynamic_cast<Polygon<TargetUnit>&>(targetGeometry);
        const Polygon<SourceUnit> &source = dynamic_cast<const Polygon<SourceUnit>&>(sourceGeometry);

        transform(source.exterior,target.exterior,project);
        for(LinearRing<SourceUnit> ring: source.interiors){
            LinearRing<TargetUnit> projected_ring;
            transform(ring,projected_ring,project);
            target.interiors.push_back(projected_ring);
        }

    } 
    
    #define TRANSFORM_GCOLLECTION(GUnit){ \
        GeometryCollection<GUnit<TargetUnit>,TargetUnit> &target = dynamic_cast<GeometryCollection<GUnit<TargetUnit>,TargetUnit>&>(targetGeometry); \
        const GeometryCollection<GUnit<SourceUnit>,SourceUnit> &source = dynamic_cast<const GeometryCollection<GUnit<SourceUnit>,SourceUnit>&>(sourceGeometry); \
        for(GUnit<SourceUnit> obj: source){ \
            GUnit<TargetUnit> projected_obj;  \
            transform(obj,projected_obj,project); \
            target.push_back(projected_obj); \
        } \
    } \

    else if(IS_GEOMETRY(sourceGeometry,MultiPoint<SourceUnit>)) TRANSFORM_GCOLLECTION(Point)
    else if(IS_GEOMETRY(sourceGeometry,MultiLineString<SourceUnit>)) TRANSFORM_GCOLLECTION(LineString)
    else if(IS_GEOMETRY(sourceGeometry,MutliPolygon<SourceUnit>)) TRANSFORM_GCOLLECTION(Polygon)
}

void Projection::project(const Geometry<LatLng> &geometry, Geometry<Point2D> &projected){
    return transform(geometry,projected,true);
}

void Projection::unproject(const Geometry<Point2D> &geometry, Geometry<LatLng> &unprojected){
    return transform(geometry,unprojected,false);
}

// ======================

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

// ======================

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


// ======================

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