#pragma once

#include <QGraphicsView>
#include <QWheelEvent>

#include "MapGraphicsScene.h"
#include "MapCamera.h"
#include "ILayer.h"
#include "Projection.h"

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

        MapGraphicsScene *scene();

    private slots:
        void onPosChanged(Point2D pos);
        void onScaleChanged(double scale);

        void onMouseMove(Point2D pos);
        void onMouseClick(Point2D pos);
        void onMouseDoubleClick(Point2D pos);

    protected:
        void wheelEvent(QWheelEvent *event) override;

    private:
        Projection *proj;
        MapCamera *cam;

        QSet<ILayer*> layerSet;

        double previousScale = 0;
};