#pragma once

#include <QGraphicsItem>

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