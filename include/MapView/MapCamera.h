#pragma once

#include <QObject>

#include "GeoTypes.hpp"


class MapGraphicsView;

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

        void clicked(LatLng pos);
        void dblClicked(LatLng pos);
        void mouseMoved(LatLng pos);

    private:
        MapGraphicsView *map;
        LatLng pos;
        double scale; // 1 physical meter per 1 projected unit
};