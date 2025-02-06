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

    Point<Point2D> ep1_projected;
    proj->project(ep1,ep1_projected);
    Point<LatLng> ep1_unprojected;
    proj->unproject(ep1_projected,ep1_unprojected);

    LineString<Point2D> line_projected;
    proj->project(line,line_projected);
    LineString<LatLng> line_unprojected;
    proj->unproject(line_projected,line_unprojected);

    Polygon<Point2D> poly_projected;
    proj->project(poly,poly_projected);
    Polygon<LatLng> poly_unprojected;
    proj->unproject(poly_projected,poly_unprojected);

    MultiPoint<Point2D> points_projected;
    proj->project(points,points_projected);
    MultiPoint<LatLng> points_unprojected;
    proj->unproject(points_projected,points_unprojected);

    MultiLineString<Point2D> lines_projected;
    proj->project(lines,lines_projected);
    MultiLineString<LatLng> lines_unprojected;
    proj->unproject(lines_projected,lines_unprojected);

    MutliPolygon<Point2D> polys_projected;
    proj->project(polys,polys_projected);
    MutliPolygon<LatLng> polys_unprojected;
    proj->unproject(polys_projected,polys_unprojected);

    return 0;
}