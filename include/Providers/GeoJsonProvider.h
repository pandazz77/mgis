#pragma once

#include <QJsonObject>
#include <QJsonDocument>
#include "Feature.h"
#include "FeatureCollection.h"

QString toStr(Geometry::Type type);
Geometry::Type fromStr(QString str);

Geometry *geometryfromVariant(QVariantMap map);

IFeature *ifeatureFromVariant(QVariantMap map);
Feature featureFromVariant(QVariantMap map);
FeatureCollection collectionFromVariant(QVariantMap map);