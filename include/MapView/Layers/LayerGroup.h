#pragma once

#include "ILayer.h"

class LayerGroup: public ILayer, public MapPane{
    Q_OBJECT

    public:
        LayerGroup(QObject *parent=nullptr);
        LayerGroup(std::initializer_list<ILayer*> layers, QObject *parent=nullptr);
        ~LayerGroup();

        QGraphicsItem *getItem() override;

    protected:
        void rebuildItem(MapPane *pane) override;
        
    private:
        QGraphicsItemGroup *group = nullptr;
};