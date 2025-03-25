#pragma once

#include "ILayer.h"
#include "Features.h"
#include "Projection.h"

class FeatureLayer: public ILayer{
    Q_OBJECT

    public:
        FeatureLayer(IFeature *feature, QObject *parent = nullptr);
        ~FeatureLayer();

        IFeature *getFeature();

        static QGraphicsItem *buildFeature(IFeature *feature,Projection *proj);
        static QGraphicsItem *buildFeatureGeometry(Geometry *geometry,Projection *proj); // ONLY SINGULAR GEOMETRY OBJECTS, NOT COLLECTION
        template<class GeometryUnit>
        static QGraphicsItemGroup *buildFeatureGeometryCollection(GeometryCollection<GeometryUnit> *collection,Projection *proj);

    protected:
        void onMapChanged() override;

    private:
        IFeature *feature;
};