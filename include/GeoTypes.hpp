#pragma once

#include <vector>
#include <typeinfo>

class Point2D{
    public:
        Point2D(double x = 0, double y = 0) : x(x), y(y) { }
        double x;
        double y;

        bool operator==(Point2D other){
            return x == other.x && y == other.y;
        }
        bool operator!=(Point2D other){
            return !operator==(other);
        }
};

class LatLng{
    public:
        LatLng(double lat = 0, double lng = 0) : lat(lat), lng(lng) { }
        double lat;
        double lng;

        bool operator==(LatLng other){
            return lat == other.lat && lng == other.lng;
        }
        bool operator!=(LatLng other){
            return !operator==(other);
        }
};

inline LatLng LngLat(double lng, double lat){
    return LatLng(lat,lng);
}

enum class GeometryType{
    POINT,
    LINESTRING,
    LINEARRING,
    POLYGON,
    MULTIPOINT,
    MULTILINESTRING,
    MULTIPOLYGON,

    BOUNDS
};

class Geometry{
    public:
        virtual GeometryType type() = 0;
};

template<class Unit = LatLng>
class Bounds: public Geometry{
    public:
        Bounds(Unit NE, Unit SW): NE(NE), SW(SW) { }

        Unit NE;
        Unit SW;

        GeometryType type() override { return GeometryType::BOUNDS; }
};

template<class Unit = LatLng>
class Point: public Geometry{
    public:
        Point(Unit pos) : coordinates(pos) { }
        Point(double val1=0, double val2=0) : Point(Unit(val1,val2)) { }

        Unit coordinates;

        GeometryType type() override{ return GeometryType::POINT; }
};

template<class Unit = LatLng>
class LineString: public Geometry, public std::vector<Unit>{
    public:
        LineString(std::vector<Unit> line={}) : std::vector<Unit>(line) { }

        GeometryType type() override{ return GeometryType::LINESTRING; }
};

template<class Unit = LatLng>
class LinearRing: public LineString<Unit>{
    public:
        LinearRing(std::vector<Unit> line={}) : LineString<Unit>(line) { 
            if(!line.empty() && LineString<Unit>::front() != LineString<Unit>::back()){
                LineString<Unit>::push_back(LineString<Unit>::front());
            }
        };
        // LinearRing(LineString<Unit> line) : LinearRing<Unit>(line) {
            
        // }
        

        GeometryType type() override{ return GeometryType::LINEARRING; };
};

template<class Unit = LatLng>
class Polygon: public Geometry{
    public:
        Polygon(LinearRing<Unit> exterior=LinearRing<Unit>(), std::vector<LinearRing<Unit>> interiors = {}) : 
            exterior(exterior), 
            interiors(interiors){ 

            }

        GeometryType type() override{ return GeometryType::POLYGON; }

        LinearRing<Unit> exterior;
        std::vector<LinearRing<Unit>> interiors;
};

template<typename GeometryUnit>
class GeometryCollection: public Geometry, public std::vector<GeometryUnit>{
    public:
        GeometryCollection(std::vector<GeometryUnit> units = {}): std::vector<GeometryUnit>(units) { }
        ~GeometryCollection(){}
};

template<class Unit = LatLng>
class MultiPoint: public GeometryCollection<Point<Unit>>{
    public:
        MultiPoint(std::vector<Point<Unit>> points = {}) : GeometryCollection<Point<Unit>>(points) { }

        GeometryType type() override{ return GeometryType::MULTIPOINT; }
};

template<class Unit = LatLng>
class MultiLineString: public GeometryCollection<LineString<Unit>>{
    public:
        MultiLineString(std::vector<LineString<Unit>> lines = {}) : GeometryCollection<LineString<Unit>>(lines) { }

        GeometryType type() override{ return GeometryType::MULTILINESTRING; }
};

template<class Unit = LatLng>
class MutliPolygon: public GeometryCollection<Polygon<Unit>>{
    public:
        MutliPolygon(std::vector<Polygon<Unit>> polygons = {}) : GeometryCollection<Polygon<Unit>>(polygons) { }

        GeometryType type() override{ return GeometryType::MULTIPOLYGON; }
};