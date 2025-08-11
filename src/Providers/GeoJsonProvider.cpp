#include "GeoJsonProvider.h"
#include <QSet>

const QSet<QPair<Geometry::Type,QString>> GEOMETRIES_STR = {
    {Geometry::Type::POINT,"Point"},
    {Geometry::Type::LINESTRING,"LineString"},
    {Geometry::Type::POLYGON,"Polygon"},
    {Geometry::Type::MULTIPOINT,"MultiPoint"},
    {Geometry::Type::MULTILINESTRING,"MultiLineString"},
    {Geometry::Type::MULTIPOLYGON,"MultiPolygon"}
};

QString GeoJsonProvider::transformGeometryType(Geometry::Type type){
    for(auto pair: GEOMETRIES_STR){
        if(pair.first==type) return pair.second;
    }
    assert(1);
}

Geometry::Type GeoJsonProvider::transformGeometryType(QString str){
    for(auto pair: GEOMETRIES_STR){
        if(pair.second==str) return pair.first;
    }
    assert(1);
}

LatLng GeoJsonProvider::transformLatLng(QVariantList lst){
    return LngLat(lst[0].toDouble(),lst[1].toDouble());
}

QVariantList GeoJsonProvider::transformLatLng(LatLng coords){
    return QVariantList({coords.lng,coords.lat});
}

LineString<LatLng> GeoJsonProvider::transformLineString(QVariantList lst){
    LineString<LatLng> result;
    for(auto pos: lst) 
        result.push_back(transformLatLng(pos.toList()));
    return result;
}

QVariantList GeoJsonProvider::transformLineString(LineString<LatLng> coords){
    QVariantList result;
    for(auto pos: coords){
        result.push_back(transformLatLng(pos));
    }
    return result;
}

std::vector<LineString<LatLng>> GeoJsonProvider::transformLineStrings(QVariantList lst){
    std::vector<LineString<LatLng>> result;
    for(auto line: lst){
        result.push_back(transformLineString(line.toList()));
    }
    return result;
}

QVariantList GeoJsonProvider::transformLineStrings(std::vector<LineString<LatLng>> coords){
    QVariantList result;
    for(auto line: coords){
        result.push_back(transformLineString(line));
    }
    return result;
}

Polygon<LatLng> GeoJsonProvider::transformPolygon(QVariantList lst){
    LineString<LatLng> exterior(transformLineString(lst[0].toList()));
    std::vector<LinearRing<LatLng>> interiors;
    for(auto interior: transformLineStrings(lst.sliced(1))){
        interiors.push_back(interior);
    }
    return Polygon<LatLng>(exterior,interiors);
}

QVariantList GeoJsonProvider::transformPolygon(Polygon<LatLng> coords){
    QVariantList exterior = transformLineString(coords.exterior);
    QVariantList interiors;
    for(auto interior: coords.interiors){
        interiors.push_back(transformLineString(interior));
    }
    return QVariantList({exterior,interiors});
}

MultiPoint<LatLng> GeoJsonProvider::transformMultiPoint(QVariantList lst){
    auto line = transformLineString(lst);
    std::vector<Point<LatLng>> points;
    for(auto p: line) points.push_back(p);
    return MultiPoint<LatLng>(points);
}

QVariantList GeoJsonProvider::transformMultiPoint(MultiPoint<LatLng> coords){
    QVariantList points;
    for(auto p: coords){
        points.push_back(transformLatLng(p.coordinates));
    }
    return points;
}


MultiLineString<LatLng> GeoJsonProvider::transformMultiLineString(QVariantList lst){
    return MultiLineString<LatLng>(transformLineStrings(lst));
}

QVariantList GeoJsonProvider::transformMultiLineString(MultiLineString<LatLng> coords){
    return transformLineStrings(coords);
}

MultiPolygon<LatLng> GeoJsonProvider::transformMultiPolygon(QVariantList lst){
    MultiPolygon<LatLng> result;
    for(QVariant poly: lst){
        result.push_back(transformPolygon(poly.toList()));
    }
    return result;
}

QVariantList GeoJsonProvider::transformMultiPolygon(MultiPolygon<LatLng> polys){
    QVariantList result;
    for(auto poly: polys){
        result.push_back(transformPolygon(poly));
    }
    return result;
}

Geometry *GeoJsonProvider::transformGeometry(QVariantMap map){
    using T = Geometry::Type;

    Geometry::Type type = transformGeometryType(map["type"].toString());
    QVariantList coords = map["coordinates"].toList(); // latlng values

    switch(type){
        case T::POINT:
            return new Point<LatLng>(transformLatLng(coords));
        case T::LINESTRING:
            return new LineString<LatLng>(transformLineString(coords));
        case T::POLYGON:
            return new Polygon<LatLng>(transformPolygon(coords));
        case T::MULTIPOINT:
            return new MultiPoint<LatLng>(transformMultiPoint(coords));
        case T::MULTILINESTRING:
            return new MultiLineString<LatLng>(transformMultiLineString(coords));
        case T::MULTIPOLYGON:
            return new MultiPolygon<LatLng>(transformMultiPolygon(coords));
    }
}

QVariantMap GeoJsonProvider::transformGeometry(Geometry *geometry){
    using T = Geometry::Type;
    QString type = transformGeometryType(geometry->type());
    QVariantList coords;

    switch(geometry->type()){
        case T::POINT:
            coords = transformLatLng(dynamic_cast<Point<LatLng>*>(geometry)->coordinates);
            break;
        case T::LINESTRING:
            coords = transformLineString(*dynamic_cast<LineString<LatLng>*>(geometry));
            break;
        case T::POLYGON:
            coords = transformPolygon(*dynamic_cast<Polygon<LatLng>*>(geometry));
            break;
        case T::MULTIPOINT:
            coords = transformMultiPoint(*dynamic_cast<MultiPoint<LatLng>*>(geometry));
            break;
        case T::MULTILINESTRING:
            coords = transformMultiLineString(*dynamic_cast<MultiLineString<LatLng>*>(geometry));
            break;
        case T::MULTIPOLYGON:
            coords = transformMultiPolygon(*dynamic_cast<MultiPolygon<LatLng>*>(geometry));
            break;
    }

    return QVariantMap({
        {"type",type},
        {"coordinates",coords}
    });
}

Feature::Properties GeoJsonProvider::transformProperites(QVariantMap properties){
    Feature::Properties result;

    for(auto kv: properties.asKeyValueRange()){
        std::string key = kv.first.toStdString();
        switch(kv.second.typeId()){
            case QMetaType::Double: result[key] = kv.second.toDouble(); break;
            case QMetaType::Int:
            case QMetaType::LongLong: result[key] = kv.second.toInt(); break;
            case QMetaType::Bool: result[key] = kv.second.toBool(); break;
            case QMetaType::QString: result[key] = kv.second.toString().toStdString(); break;
            /// TODO: enhance Feature::properties  
        }
    }

    return result;
}

QVariantMap GeoJsonProvider::transformProperties(FProps properties){
    QVariantMap result;
    for(auto kv: properties){
        QString key = QString::fromStdString(kv.first);
        if(std::holds_alternative<int>(kv.second)) result[key] = std::get<int>(kv.second);
        else if(std::holds_alternative<double>(kv.second)) result[key] = std::get<double>(kv.second);
        else if(std::holds_alternative<bool>(kv.second)) result[key] = std::get<bool>(kv.second);
        else if(std::holds_alternative<std::string>(kv.second)) result[key] = QString::fromStdString(std::get<std::string>(kv.second));
    }

    return result;
}


Feature GeoJsonProvider::transformFeature(QVariantMap map){
    Feature feature(transformGeometry(map["geometry"].toMap()));
    feature.properties = transformProperites(map["properties"].toMap());

    return feature;
}

QVariantMap GeoJsonProvider::transformFeature(const Feature &feature){
    QVariantMap geometry = transformGeometry(feature.geometry);
    QVariantMap properties = transformProperties(feature.properties);

    return QVariantMap({
        {"type","Feature"},
        {"geometry",geometry},
        {"properties",properties}
    });
}

FeatureCollection GeoJsonProvider::transformCollection(QVariantMap map){
    FeatureCollection collection;
    QVariantList var_features = map["features"].toList();
    for(auto var_feature: var_features){
        collection.push_back(transformIFeature(var_feature.toMap()));
    }
    return collection;
}

QVariantMap GeoJsonProvider::transformCollection(const FeatureCollection &collection){
    QVariantList features;
    for(IFeature* feature: collection){
        features.push_back(transformIFeature(feature));
    }
    return QVariantMap{
        {"type","FeatureCollection"},
        {"features",features}
    };
}

IFeature *GeoJsonProvider::transformIFeature(QVariantMap map){
    QString type = map["type"].toString();
    IFeature *result = nullptr;
    if(type == "Feature") result = new Feature(transformFeature(map));
    else if(type == "FeatureCollection") result = new FeatureCollection(transformCollection(map));
    return result;
}

QVariantMap GeoJsonProvider::transformIFeature(IFeature *feature){
    if(dynamic_cast<Feature*>(feature)) return transformFeature(*dynamic_cast<Feature*>(feature));
    else if(dynamic_cast<FeatureCollection*>(feature)) return transformCollection(*dynamic_cast<FeatureCollection*>(feature));
}