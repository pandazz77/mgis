#pragma once

#include "LayerSet.hpp"

class MapGraphicsView;
class ILayer;

class MapPane: public LayerSet<ILayer>{
    public:
        MapPane(MapPane *parent=nullptr);

        virtual MapGraphicsView *map();

    protected:
        MapPane *parentPane();
        void setParentPane(MapPane *parent);

    private:
        MapPane *parent = nullptr;
};