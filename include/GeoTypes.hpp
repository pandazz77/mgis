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

template<class PosUnit>
class Geometry{
    public:
        Geometry(): postype(typeid(PosUnit)) {}
        virtual ~Geometry() {};

        virtual GeometryType type() = 0;

        const std::type_info &postype;
};

template<class PosUnit = LatLng>
class Bounds: public Geometry<PosUnit>{
    public:
        Bounds(PosUnit NE, PosUnit SW): NE(NE), SW(SW) { }
        PosUnit NE;
        PosUnit SW;

        GeometryType type() override { return GeometryType::BOUNDS; }
};

template<class GeometryUnit, class PosUnit>
class GeometryObj: public Geometry<PosUnit>{
    public:
        GeometryObj(GeometryUnit coords) : coordinates(coords) { }

        GeometryObj<GeometryUnit,PosUnit> &operator =(const GeometryObj<GeometryUnit,PosUnit> &other) { 
            this->coordinates = other.coordinates;
            return *this;
        }

        // operator(const GeometryObj &other) { this = other; }

        GeometryUnit coordinates;
        ~GeometryObj(){}
};

template<class Unit = LatLng>
class Point: public GeometryObj<Unit,Unit>{
    public:
        Point(Unit pos) : GeometryObj<Unit,Unit>(pos) { }
        Point(double val1=0, double val2=0) : Point(Unit(val1,val2)) { }

        GeometryType type() override{ return GeometryType::POINT; }
};

template<class Unit = LatLng>
class LineString: public GeometryObj<std::vector<Unit>,Unit>{
    public:
        LineString(std::vector<Unit> line={}) : GeometryObj<std::vector<Unit>,Unit>(line) { }

        GeometryType type() override{ return GeometryType::LINESTRING; }
};

template<class Unit = LatLng>
class LinearRing: public LineString<Unit>{
    public:
        LinearRing(std::vector<Unit> line={}) : LineString<Unit>(line) { 
            if(!line.empty() && LineString<Unit>::coordinates.front() != LineString<Unit>::coordinates.back()){
                LineString<Unit>::coordinates.push_back(LineString<Unit>::coordinates.front());
            }
        };
        LinearRing(LineString<Unit> line) : LinearRing<Unit>(line.coordinates) {
            
        }
        

        GeometryType type() override{ return GeometryType::LINEARRING; };
};

template<class Unit = LatLng>
class Polygon: public GeometryObj<std::pair <LinearRing<Unit>, std::vector<LinearRing<Unit>> >,Unit>{
    public:
        Polygon(LinearRing<Unit> exterior=LinearRing<Unit>(), std::vector<LinearRing<Unit>> interiors = {}) : 
            GeometryObj<std::pair <LinearRing<Unit>, std::vector<LinearRing<Unit>> >,Unit>({exterior,interiors}),
            exterior(GeometryObj<std::pair <LinearRing<Unit>, std::vector<LinearRing<Unit>> >,Unit>::coordinates.first), 
            interiors(GeometryObj<std::pair <LinearRing<Unit>, std::vector<LinearRing<Unit>> >,Unit>::coordinates.second){ 

            }

        GeometryType type() override{ return GeometryType::POLYGON; }

        LinearRing<Unit> &exterior;
        std::vector<LinearRing<Unit>> &interiors;
};

template<typename geometryUnit, class Unit>
class GeometryCollection: public Geometry<Unit>, public std::vector<geometryUnit>{
    public:
        GeometryCollection(std::vector<geometryUnit> units): std::vector<geometryUnit>(units) { }
        ~GeometryCollection(){}
};

template<class Unit = LatLng>
class MultiPoint: public GeometryCollection<Point<Unit>,Unit>{
    public:
        MultiPoint(std::vector<Point<Unit>> points = {}) : GeometryCollection<Point<Unit>,Unit>(points) { }

        GeometryType type() override{ return GeometryType::MULTIPOINT; }
};

template<class Unit = LatLng>
class MultiLineString: public GeometryCollection<LineString<Unit>,Unit>{
    public:
        MultiLineString(std::vector<LineString<Unit>> lines = {}) : GeometryCollection<LineString<Unit>,Unit>(lines) { }

        GeometryType type() override{ return GeometryType::MULTILINESTRING; }
};

template<class Unit = LatLng>
class MutliPolygon: public GeometryCollection<Polygon<Unit>,Unit>{
    public:
        MutliPolygon(std::vector<Polygon<Unit>> polygons = {}) : GeometryCollection<Polygon<Unit>,Unit>(polygons) { }

        GeometryType type() override{ return GeometryType::MULTIPOLYGON; }
};