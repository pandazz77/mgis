#pragma once

#include <QJsonObject>
#include <QJsonDocument>
#include "Feature.h"
#include "FeatureCollection.h"

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

        static Feature::Properties transformProperites(QVariantMap properties);
        static QVariantMap transformProperties(Feature::Properties properties);

        static Geometry *transformGeometry(QVariantMap map);
        static QVariantMap transformGeometry(Geometry *geometry);
        static Feature transformFeature(QVariantMap map);
        static QVariantMap transformFeature(const Feature &feature);
        static FeatureCollection transformCollection(QVariantMap map);
        static QVariantMap transformCollection(const FeatureCollection &collection);
        static IFeature *transformIFeature(QVariantMap map);
        static QVariantMap transformIFeature(IFeature *feature);
};