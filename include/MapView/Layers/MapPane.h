#pragma once

#include <QSet>

class MapGraphicsView;
class ILayer;

class MapPane{
    public:
        MapPane(MapPane *parent=nullptr);

        virtual void addLayer(ILayer *layer);
        virtual void removeLayer(ILayer *layer);
        QSet<ILayer*> getLayers();

        virtual MapGraphicsView *map();

    protected:
        MapPane *parentPane();
        void setParentPane(MapPane *parent);

    protected:
        QSet<ILayer *> layers;

    private:
        MapPane *parent = nullptr;
};