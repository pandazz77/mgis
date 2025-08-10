#pragma once

#include <QGraphicsView>
#include <QWheelEvent>

#include "MapGraphicsScene.h"
#include "MapCamera.h"
#include "MapPane.h"
#include "ILayer.h"
#include "Projection.h"

class MapGraphicsView: public QGraphicsView, public MapPane{
    Q_OBJECT

    public:
        MapGraphicsView(QWidget *parent = nullptr);
        ~MapGraphicsView();

        void setProjection(Projection *proj);
        Projection *getProjection();

        MapCamera *getCam();

        void addLayer(ILayer *layer) override;
        void removeLayer(ILayer *layer) override;

        MapGraphicsView *map() override;
        MapPane *parentPane() = delete;
        void setParentPane(MapPane *parent) = delete;

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

        double previousScale = 0;
};