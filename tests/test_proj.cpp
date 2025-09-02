#include <cassert>
#include <iostream>

#include "Projection.h"
#include "Mercator.h"
#include "SphericalMercator.h"

void ASSERT_LATLNG_EQUAL(const LatLng &ll1, const LatLng &ll2, const double delta=6e-9){
    double latDif = std::abs(ll1.lat - ll2.lat);
    double lngDif = std::abs(ll1.lng - ll2.lng);
    assert(latDif <= delta && lngDif <= delta);
}

void ASSERT_POINT2D_EQUAL(const Point2D &p1, const Point2D &p2, const double delta=6e-9){
    double xDif = std::abs(p1.x - p2.x);
    double yDif = std::abs(p1.y - p2.y);
    assert(xDif <= delta && yDif <= delta);
}

template<typename Unit>
void ASSERT_UNIT_EQUAL(const Unit &u1, const Unit &u2, const double delta = 6e-9){
    if constexpr (std::is_same_v<Unit, LatLng>) ASSERT_LATLNG_EQUAL(u1,u2,delta);
    else if constexpr (std::is_same_v<Unit, Point2D>) ASSERT_POINT2D_EQUAL(u1,u2,delta); 
}

template<typename T>
void ASSERT_POINT_EQUAL(const Point<T> &p1, const Point<T> &p2, const double delta = 6e-9) {
    ASSERT_UNIT_EQUAL(p1.coordinates,p2.coordinates,delta);
}

template<typename T>
void ASSERT_LINE_EQUAL(const LineString<T> &l1, const LineString<T> &l2, const double delta = 6e-9) {
    assert(l1.size()==l2.size());
    for(int i=0; i < l1.size(); i++)
        ASSERT_UNIT_EQUAL(l1[i],l2[i],delta);
}

template<typename T>
void ASSERT_POLY_EQUAL(const Polygon<T> &poly1, const Polygon<T> &poly2, const double delta = 6e-9) {
    ASSERT_LINE_EQUAL(poly1.exterior,poly1.exterior,delta);
    assert(poly1.interiors.size()==poly2.interiors.size());
    for(int i=0; i < poly1.interiors.size(); i++)
        ASSERT_LINE_EQUAL(poly1.interiors[i],poly2.interiors[i],delta);
}

template<typename Unit>
void ASSERT_COLLECTION_EQUAL(const GeometryCollection<Unit> &col1, const GeometryCollection<Unit> &col2, const double delta = 6e-9){
    assert(col1.size()==col2.size());
    for(int i=0; i < col1.size(); i++){
        if constexpr(std::is_same_v<Unit,Polygon<>>) ASSERT_POLY_EQUAL(col1[i],col2[i]);
    }
}

int main(int argc, char *argv[]){
    Point ep1(47.51626409685045, 69.645822358462);
    Point ep2(47.51626409685045, 68.5207540327172);
    Point ep3(50.99910796212228, 68.5207540327172);
    Point ep4(50.99910796212228, 69.645822358462);
    Point ep5(47.51626409685045, 69.645822358462);

    Point ip1(48.52912652090629,69.30113643786777);
    Point ip2(48.52912652090629,68.86238464139848);
    Point ip3(49.90247868793995,68.86238464139848);
    Point ip4(49.90247868793995,69.30113643786777);
    Point ip5(48.52912652090629,69.30113643786777);

    LineString line({ep1.coordinates,ep2.coordinates,ep3.coordinates,ep4.coordinates,ep5.coordinates});
    LinearRing ring(line);

    Polygon poly(ring,{
        LinearRing({
            ip1.coordinates,
            ip2.coordinates,
            ip3.coordinates,
            ip4.coordinates,
            ip5.coordinates
        }),
    });

    MultiPoint points({ep1,ep2});
    MultiLineString lines({line,ring});
    MultiPolygon polys({poly,poly});

    // ==========

    Projection *proj = new SphericalMercator();
    Projection *merc = new Mercator();

    LatLng ll1(ep1.coordinates);
    
    Point2D ll1_projected = proj->project(ll1);
    LatLng ll1_unprojected = proj->unproject(ll1_projected);
    ASSERT_UNIT_EQUAL(ll1,ll1_unprojected);

    Point2D ll1_mercproj = merc->project(ll1);
    LatLng ll1_mercunproj = merc->unproject(ll1_mercproj);
    ASSERT_UNIT_EQUAL(ll1,ll1_mercunproj);

    Point<Point2D> ep1_projected = proj->transform<LatLng,Point2D>(ep1);
    Point<LatLng> ep1_unprojected = proj->transform<Point2D,LatLng>(ep1_projected);
    ASSERT_POINT_EQUAL(ep1, ep1_unprojected);

    LineString<Point2D> line_projected = proj->transform<LatLng,Point2D>(line);
    LineString<LatLng> line_unprojected = proj->transform<Point2D,LatLng>(line_projected);
    ASSERT_LINE_EQUAL(line,line_unprojected);

    Polygon<Point2D> poly_projected = proj->transform<LatLng,Point2D>(poly);
    Polygon<LatLng> poly_unprojected = proj->transform<Point2D,LatLng>(poly_projected);
    ASSERT_POLY_EQUAL(poly, poly_unprojected);

    MultiPoint<Point2D> points_projected = proj->transform<LatLng,Point2D>(points);
    MultiPoint<LatLng> points_unprojected = proj->transform<Point2D,LatLng>(points_projected);
    ASSERT_COLLECTION_EQUAL(points,points_unprojected);

    MultiLineString<Point2D> lines_projected = proj->transform<LatLng,Point2D>(lines);
    MultiLineString<LatLng> lines_unprojected = proj->transform<Point2D,LatLng>(lines_projected);
    ASSERT_COLLECTION_EQUAL(lines,lines_unprojected);

    MultiPolygon<Point2D> polys_projected = proj->transform<LatLng,Point2D>(polys);
    MultiPolygon<LatLng> polys_unprojected = proj->transform<Point2D,LatLng>(polys_projected);
    ASSERT_COLLECTION_EQUAL(polys,polys_unprojected);


    Point<Point2D> pt_pr = proj->project(ep1_unprojected);
    LineString<Point2D> ls_pr = proj->project(line);
    LinearRing<Point2D> lr_pr = proj->project(LinearRing(line));
    Polygon<Point2D> pl_pr = proj->project(poly);
    MultiPoint<Point2D> mpt_pr = proj->project(points);
    MultiLineString<Point2D> mls_pr = proj->project(lines);
    MultiPolygon<Point2D> mpl_pr = proj->project(polys);

    Point<LatLng> pt_unpr = proj->unproject(pt_pr);
    LineString<LatLng> ls_unpr = proj->unproject(ls_pr);
    LinearRing<LatLng> lr_unpr = proj->unproject(lr_pr);
    Polygon<LatLng> pl_unpr = proj->unproject(pl_pr);
    MultiPoint<LatLng> mpt_unpr = proj->unproject(mpt_pr);
    MultiLineString<LatLng> mls_unpr = proj->unproject(mls_pr);
    MultiPolygon<LatLng> mpl_unpr = proj->unproject(mpl_pr);

    return 0;
}