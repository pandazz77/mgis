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

class Geometry{
    public:
        enum class Type{
            POINT,
            LINESTRING,
            POLYGON,
            MULTIPOINT,
            MULTILINESTRING,
            MULTIPOLYGON,

            BOUNDS
        };

        virtual Geometry::Type type() = 0;
};

template<class Unit = LatLng>
class Bounds: public Geometry{
    public:
        Bounds(Unit NE, Unit SW): NE(NE), SW(SW) { }

        Unit NE;
        Unit SW;

        Geometry::Type type() override { return Geometry::Type::BOUNDS; }
};

template<class Unit = LatLng>
class Point: public Geometry{
    public:
        Point(Unit pos) : coordinates(pos) { }
        Point(double val1=0, double val2=0) : Point(Unit(val1,val2)) { }

        Unit coordinates;

        Geometry::Type type() override{ return Geometry::Type::POINT; }
};

template<class Unit = LatLng>
class LineString: public Geometry, public std::vector<Unit>{
    public:
        LineString(std::vector<Unit> line = {}): std::vector<Unit>(line) { }
        LineString(std::initializer_list<Unit> line) : LineString(std::vector<Unit>(std::move(line))) { }

        Geometry::Type type() override{ return Geometry::Type::LINESTRING; }
};

namespace LinearRing{
    template<class Unit = LatLng>
    bool enclosed(const LineString<Unit> &line){
        return line.cbegin() == line.cend(); 
    }

    template<class Unit = LatLng>
    void enclose(LineString<Unit> &line){
        if(!enclosed(line)) line.push_back(line.front());
    }
};

template<class Unit = LatLng>
class Polygon: public Geometry{
    public:
        Polygon(LineString<Unit> exterior={}, std::vector<LineString<Unit>> interiors = {}) : 
            exterior(exterior), 
            interiors(interiors){ 
                LinearRing::enclose(exterior);
                for(auto i: interiors) LinearRing::enclose(i);
            }

        Geometry::Type type() override{ return Geometry::Type::POLYGON; }

        LineString<Unit> exterior;
        std::vector<LineString<Unit>> interiors;
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
        MultiPoint(std::initializer_list<Point<Unit>> points) : MultiPoint(std::vector<Point<Unit>>(points)) { }

        Geometry::Type type() override{ return Geometry::Type::MULTIPOINT; }
};

template<class Unit = LatLng>
class MultiLineString: public GeometryCollection<LineString<Unit>>{
    public:
        MultiLineString(std::vector<LineString<Unit>> lines = {}) : GeometryCollection<LineString<Unit>>(lines) { }
        MultiLineString(std::initializer_list<LineString<Unit>> points) : MultiLineString(std::vector<LineString<Unit>>(points)) { }

        Geometry::Type type() override{ return Geometry::Type::MULTILINESTRING; }
};

template<class Unit = LatLng>
class MultiPolygon: public GeometryCollection<Polygon<Unit>>{
    public:
        MultiPolygon(std::vector<Polygon<Unit>> polygons = {}) : GeometryCollection<Polygon<Unit>>(polygons) { }
        MultiPolygon(std::initializer_list<Polygon<Unit>> points) : MultiPolygon(std::vector<Polygon<Unit>>(points)) { }

        Geometry::Type type() override{ return Geometry::Type::MULTIPOLYGON; }
};