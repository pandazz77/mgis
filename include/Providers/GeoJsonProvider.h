#pragma once

#include <QJsonObject>
#include <QJsonDocument>
#include "Feature.h"
#include "FeatureCollection.h"

class GeoJsonProvider{
    public:


        static LatLng transformLatLng(QVariantList coords);
        static LineString<LatLng> transformLineString(QVariantList coords);
        static std::vector<LineString<LatLng>> transformLineStrings(QVariantList coords);
        static Polygon<LatLng> transformPolygon(QVariantList coords);
        static MultiPoint<LatLng> transformMultiPoint(QVariantList coords);
        static MultiLineString<LatLng> transformMultiLineString(QVariantList coords);
        static MultiPolygon<LatLng> transformMultiPolygon(QVariantList coords);

        static QString transformGeometryType(Geometry::Type type);
        static Geometry::Type transformGeometryType(QString type);

        static Geometry *geometryfromVariant(QVariantMap map);
        static IFeature *ifeatureFromVariant(QVariantMap map);
        static Feature featureFromVariant(QVariantMap map);
        static FeatureCollection collectionFromVariant(QVariantMap map);
};