#pragma once

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QWidget>
#include <QWheelEvent>

#include "Projection.h"
#include "Features.h"

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

    protected:
        void onMapChanged() override;

    private:
        IFeature *feature;
};

class MapCamera: public QObject{
    Q_OBJECT

    public:
        MapCamera(LatLng pos, double scale, MapGraphicsView *map = nullptr);
        ~MapCamera();

        Point2D getProjectedPos();
        LatLng getPos();
        double getScale();

        void setProjectedPos(Point2D pos);
        void setPos(LatLng pos);
        void setScale(double scale);

        void zoomBy(double zoom);

    signals:
        void posChanged(LatLng pos);
        void projectedPosChanged(Point2D pos);
        void scaleChanged(double scale);

    private:
        MapGraphicsView *map;
        LatLng pos;
        double scale; // 1 physical meter per 1 projected unit
};

class MapGraphicsView: public QGraphicsView{
    Q_OBJECT

    public:
        MapGraphicsView(QWidget *parent = nullptr);
        ~MapGraphicsView();

        void setProjection(Projection *proj);
        Projection *getProjection();

        MapCamera *getCam();

        void addLayer(ILayer *layer);
        void removeLayer(ILayer *layer);

    private slots:
        void onPosChanged(Point2D pos);
        void onScaleChanged(double scale);

    protected:
        void wheelEvent(QWheelEvent *event) override;

    private:
        Projection *proj;
        MapCamera *cam;

        QSet<ILayer*> layerSet;

        double previousScale = 0;
};