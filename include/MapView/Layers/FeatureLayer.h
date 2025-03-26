#pragma once

#include "ILayer.h"
#include "Features.h"
#include "Projection.h"

using FGraphicsItem         = QGraphicsItem;
using FGraphicsPoint        = QGraphicsPixmapItem;
using FGraphicsLineString   = QGraphicsPathItem;
using FGraphicsPolygon      = QGraphicsPathItem;
using FGraphicsCollection   = QGraphicsItemGroup;

class FeatureLayer: public ILayer{
    Q_OBJECT

    public:
        FeatureLayer(IFeature *feature, QObject *parent = nullptr);
        ~FeatureLayer();

        IFeature *getFeature();

        static FGraphicsItem *buildFeature(IFeature *feature,Projection *proj);
        static FGraphicsItem *buildFeatureGeometry(Geometry *geometry,Projection *proj); // ONLY SINGULAR GEOMETRY OBJECTS, NOT COLLECTION
        template<class GeometryUnit>
        static FGraphicsCollection *buildFeatureGeometryCollection(GeometryCollection<GeometryUnit> *collection,Projection *proj);

    protected:
        void onMapChanged() override;

    private:
        IFeature *feature;
};