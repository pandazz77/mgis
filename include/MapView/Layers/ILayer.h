#pragma once

#include <QGraphicsItem>

class MapGraphicsView;

class ILayer: public QObject{
    Q_OBJECT

    public:
        ILayer(QObject *parent = nullptr);
        virtual ~ILayer() = default;

        virtual QGraphicsItem *getItem() = 0;
        void addTo(MapGraphicsView *map);
        void removeFrom(MapGraphicsView *map);

    protected:
        virtual void onMapChanged() = 0;

    protected:
        MapGraphicsView *map = nullptr;
};