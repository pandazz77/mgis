#include "GeoJsonProvider.h"
#include <QSet>

// latlng <-> lnglat
QVariantList wrapCoords(QVariantList coords){
    if(coords[0].typeId() != QMetaType::QVariantList) {
        return QVariantList{coords[1], coords[0]};
    } else {
        QVariantList result;
        for(QVariant item : coords)
            result.push_back(wrapCoords(item.toList()));
        return result;
    }
}

const QSet<QPair<Geometry::Type,QString>> GEOMETRIES_STR = {
    {Geometry::Type::POINT,"Point"},
    {Geometry::Type::LINESTRING,"LineString"},
    {Geometry::Type::POLYGON,"Polygon"},
    {Geometry::Type::MULTIPOINT,"MultiPoint"},
    {Geometry::Type::MULTILINESTRING,"MultiLineString"},
    {Geometry::Type::MULTIPOLYGON,"MultiPolygon"}
};

QString toStr(Geometry::Type type){
    for(auto pair: GEOMETRIES_STR){
        if(pair.first==type) return pair.second;
    }
    assert(1);
}

Geometry::Type fromStr(QString str){
    for(auto pair: GEOMETRIES_STR){
        if(pair.second==str) return pair.first;
    }
    assert(1);
}

LatLng fetchLatLng(QVariantList lst){
    return LatLng(lst[0].toDouble(),lst[1].toDouble());
}

LineString<LatLng> fetchLineString(QVariantList lst){
    LineString<LatLng> result;
    for(auto pos: lst) 
        result.push_back(fetchLatLng(pos.toList()));
    return result;
}

std::vector<LineString<LatLng>> fetchLineStrings(QVariantList lst){
    std::vector<LineString<LatLng>> result;
    for(auto line: lst){
        result.push_back(fetchLineString(line.toList()));
    }
    return result;
}

Polygon<LatLng> fetchPolygon(QVariantList lst){
    LineString<LatLng> exterior(fetchLineString(lst[0].toList()));
    std::vector<LinearRing<LatLng>> interiors;
    for(auto interior: fetchLineStrings(lst.sliced(1))){
        interiors.push_back(interior);
    }
    return Polygon<LatLng>(exterior,interiors);
}

MultiPoint<LatLng> fetchMultiPoint(QVariantList lst){
    auto line = fetchLineString(lst);
    std::vector<Point<LatLng>> points;
    for(auto p: line) points.push_back(p);
    return MultiPoint<LatLng>(points);
}

MultiLineString<LatLng> fetchMutliLineString(QVariantList lst){
    return MultiLineString<LatLng>(fetchLineStrings(lst));
}

MultiPolygon<LatLng> fetchMultiPolygon(QVariantList lst){
    MultiPolygon<LatLng> result;
    for(QVariant poly: lst){
        result.push_back(fetchPolygon(poly.toList()));
    }
    return result;
}

Geometry *geometryfromVariant(QVariantMap map){
    using T = Geometry::Type;

    Geometry::Type type = fromStr(map["type"].toString());
    QVariantList coords = wrapCoords(map["coordinates"].toList()); // latlng values

    switch(type){
        case T::POINT:
            return new Point<LatLng>(fetchLatLng(coords));
        case T::LINESTRING:
            return new LineString<LatLng>(fetchLineString(coords));
        case T::POLYGON:
            return new Polygon<LatLng>(fetchPolygon(coords));
        case T::MULTIPOINT:
            return new MultiPoint<LatLng>(fetchMultiPoint(coords));
        case T::MULTILINESTRING:
            return new MultiLineString<LatLng>(fetchMutliLineString(coords));
        case T::MULTIPOLYGON:
            return new MultiPolygon<LatLng>(fetchMultiPolygon(coords));
    }
}

Feature featureFromVariant(QVariantMap map){
    Feature feature(geometryfromVariant(map["geometry"].toMap()));
    QVariantMap var_props = map["properties"].toMap();

    std::unordered_map<std::string,std::variant<int,double,bool,std::string>> properties;

    for(auto kv: var_props.asKeyValueRange()){
        std::string key = kv.first.toStdString();
        switch(kv.second.typeId()){
            case QMetaType::Int: feature.properties[key] = kv.second.toInt();
            case QMetaType::Double: feature.properties[key] = kv.second.toDouble();
            case QMetaType::Bool: feature.properties[key] = kv.second.toBool();
            case QMetaType::QString: feature.properties[key] = kv.second.toString().toStdString();
            /// TODO: enhance Feature::properties  
        }
    }

    return feature;
}

FeatureCollection collectionFromVariant(QVariantMap map){
    FeatureCollection collection;
    QVariantList var_features = map["features"].toList();
    for(auto var_feature: var_features){
        collection.push_back(ifeatureFromVariant(var_feature.toMap()));
    }
    return collection;
}

IFeature *ifeatureFromVariant(QVariantMap map){
    QString type = map["type"].toString();
    IFeature *result = nullptr;
    if(type == "Feature") result = new Feature(featureFromVariant(map));
    else if(type == "FeatureCollection") result = new FeatureCollection(collectionFromVariant(map));
    return result;
}