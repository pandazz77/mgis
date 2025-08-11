#pragma once

#include <QJsonObject>
#include <QJsonDocument>
#include "Feature.h"
#include "FeatureCollection.h"

using FProps = std::unordered_map<std::string,std::variant<int,double,bool,std::string>>;

class GeoJsonProvider{
    public:

        static QString transformGeometryType(Geometry::Type type);
        static Geometry::Type transformGeometryType(QString type);

        static LatLng transformLatLng(QVariantList coords);
        static QVariantList transformLatLng(LatLng coords);
        static LineString<LatLng> transformLineString(QVariantList coords);
        static QVariantList transformLineString(LineString<LatLng> coords);
        static std::vector<LineString<LatLng>> transformLineStrings(QVariantList coords);
        static QVariantList transformLineStrings(std::vector<LineString<LatLng>> coords);
        static Polygon<LatLng> transformPolygon(QVariantList coords);
        static QVariantList transformPolygon(Polygon<LatLng> coords);
        static MultiPoint<LatLng> transformMultiPoint(QVariantList coords);
        static QVariantList transformMultiPoint(MultiPoint<LatLng> coords);
        static MultiLineString<LatLng> transformMultiLineString(QVariantList coords);
        static QVariantList transformMultiLineString(MultiLineString<LatLng> coords);
        static MultiPolygon<LatLng> transformMultiPolygon(QVariantList coords);
        static QVariantList transformMultiPolygon(MultiPolygon<LatLng> coords);

        static FProps transformProperites(QVariantMap properties);
        static QVariantMap transformProperties(FProps properties);

        static Geometry *geometryfromVariant(QVariantMap map);
        static QVariantMap geometryToVariant(Geometry *geometry);
        static Feature featureFromVariant(QVariantMap map);
        static QVariantMap featureToVariant(const Feature &feature);
        static FeatureCollection collectionFromVariant(QVariantMap map);
        static QVariantMap collectionToVariant(const FeatureCollection &collection);
        static IFeature *ifeatureFromVariant(QVariantMap map);
        static QVariantMap ifeatureToVariant(IFeature *feature);
};