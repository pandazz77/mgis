#pragma once

class MapGraphicsView;

class MapPane{
    public:
        MapPane(MapPane *parent=nullptr);

        virtual MapGraphicsView *map();
        MapPane *parentPane();
        void setParentPane(MapPane *parent);

    private:
        MapPane *parent = nullptr;
};