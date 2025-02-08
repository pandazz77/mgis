#include <cassert>
#include <iostream>

#include "Projection.h"

int main(int argc, char *argv){
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
    MutliPolygon polys({poly,poly});

    // ==========

    Projection *proj = new SphericalMercator();
    Projection *merc = new Mercator();

    LatLng ll1(ep1.coordinates);
    
    Point2D ll1_projected = proj->project(ll1);
    LatLng ll1_unprojected = proj->unproject(ll1_projected);
    //assert(ll1 == ll1_unprojected);

    Point2D ll1_mercproj = merc->project(ll1);
    LatLng ll1_mercunproj = merc->unproject(ll1_mercproj);
    //assert(ll1 == ll1_mercunproj);

    Point<Point2D> ep1_projected = proj->transform<LatLng,Point2D>(ep1);
    Point<LatLng> ep1_unprojected = proj->transform<Point2D,LatLng>(ep1_projected);

    LineString<Point2D> line_projected = proj->transform<LatLng,Point2D>(line);
    LineString<LatLng> line_unprojected = proj->transform<Point2D,LatLng>(line_projected);

    Polygon<Point2D> poly_projected = proj->transform<LatLng,Point2D>(poly);
    Polygon<LatLng> poly_unprojected = proj->transform<Point2D,LatLng>(poly_projected);

    MultiPoint<Point2D> points_projected = proj->transform<LatLng,Point2D>(points);
    MultiPoint<LatLng> points_unprojected = proj->transform<Point2D,LatLng>(points_projected);

    MultiLineString<Point2D> lines_projected = proj->transform<LatLng,Point2D>(lines);
    MultiLineString<LatLng> lines_unprojected = proj->transform<Point2D,LatLng>(lines_projected);

    MutliPolygon<Point2D> polys_projected = proj->transform<LatLng,Point2D>(polys);
    MutliPolygon<LatLng> polys_unprojected = proj->transform<Point2D,LatLng>(polys_projected);


    Point<Point2D> pt_pr = proj->project(ep1_unprojected);
    LineString<Point2D> ls_pr = proj->project(line);
    LinearRing<Point2D> lr_pr = proj->project(LinearRing(line));
    Polygon<Point2D> pl_pr = proj->project(poly);
    MultiPoint<Point2D> mpt_pr = proj->project(points);
    MultiLineString<Point2D> mls_pr = proj->project(lines);
    MutliPolygon<Point2D> mpl_pr = proj->project(polys);

    Point<LatLng> pt_unpr = proj->unproject(pt_pr);
    LineString<LatLng> ls_unpr = proj->unproject(ls_pr);
    LinearRing<LatLng> lr_unpr = proj->unproject(lr_pr);
    Polygon<LatLng> pl_unpr = proj->unproject(pl_pr);
    MultiPoint<LatLng> mpt_unpr = proj->unproject(mpt_pr);
    MultiLineString<LatLng> mls_unpr = proj->unproject(mls_pr);
    MutliPolygon<LatLng> mpl_unpr = proj->unproject(mpl_pr);

    return 0;
}