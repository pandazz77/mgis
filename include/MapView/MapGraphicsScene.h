#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include "GeoTypes.hpp"

class MapGraphicsScene: public QGraphicsScene{
    Q_OBJECT

    public:
        MapGraphicsScene(QObject *parent=nullptr);
        ~MapGraphicsScene();

    signals:
        void mouseMoved(Point2D pos);
        void clicked(Point2D pos);
        void doubleClicked(Point2D pos);

    protected:
        void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
};