#pragma once

#include "IFeatureLayer.h"
#include "Feature.h"
#include "Projection.h"
#include "Stylers.h"

using FGraphicsItem         = QGraphicsItem;
using FGraphicsPoint        = QGraphicsPixmapItem;
using FGraphicsLineString   = QGraphicsPathItem;
using FGraphicsPolygon      = QGraphicsPathItem;
using FGraphicsCollection   = QGraphicsItemGroup;

class FeatureLayer: public IFeatureLayer{
    Q_OBJECT

    public:
        FeatureLayer(Feature *feature, QObject *parent = nullptr);
        ~FeatureLayer();

        Feature *getFeature() override;

        QGraphicsItem *getItem() override;

        IStyler *styler = RandomStyler::getInstance(); /// TODO: styler field as shared ptr 

    protected:
        static FGraphicsItem *buildFeature(Feature *feature,Projection *proj);
        static FGraphicsItem *buildFeatureGeometry(Geometry *geometry,Projection *proj); // ONLY SINGULAR GEOMETRY OBJECTS, NOT COLLECTION
        template<class GeometryUnit>
        static FGraphicsCollection *buildFeatureGeometryCollection(GeometryCollection<GeometryUnit> *collection,Projection *proj);

        void rebuildItem(MapPane *pane) override;

    private:
        FGraphicsItem *item = nullptr;
        Feature *feature;
};