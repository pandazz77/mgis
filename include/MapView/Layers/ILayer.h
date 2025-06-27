#pragma once

#include <QGraphicsItem>
#include "MapPane.h"

class ILayer: public QObject{
    Q_OBJECT

    public:
        ILayer(QObject *parent = nullptr);
        virtual ~ILayer() = default;

        virtual QGraphicsItem *getItem() = 0;
        void addTo(MapPane *pane);
        void removeFrom(MapPane *pane);
        
        virtual void rebuildItem() = 0;

    protected:
        MapPane *pane = nullptr;
};