#pragma once

#include "Features.h"
#include "Projection.h"

class MapGraphicsView;

class ILayer: public QObject{
    Q_OBJECT

    public:
        ILayer(QObject *parent = nullptr);
        virtual ~ILayer() = default;

        QGraphicsItem *getItem();
        void addTo(MapGraphicsView *map);
        void removeFrom(MapGraphicsView *map);

    protected:
        virtual void onMapChanged() = 0;

    protected:
        QGraphicsItem *item = nullptr;
        MapGraphicsView *map = nullptr;
};

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