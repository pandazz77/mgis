#pragma once

#include "ILayer.h"

class LayerGroup: public ILayer, public MapPane{
    Q_OBJECT

    public:
        LayerGroup(std::initializer_list<ILayer*> layers = {}, QObject *parent=nullptr);
        ~LayerGroup();

        void addLayer(ILayer *layer);
        void removeLayer(ILayer *layer);
        QSet<ILayer*> getLayers();

        QGraphicsItem *getItem() override;

    protected:
        void rebuildItem() override;
        
    private:
        QGraphicsItemGroup *group = nullptr;
        QSet<ILayer*> layers;
};