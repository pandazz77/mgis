#pragma once

#include "ILayer.h"
#include "TLayerGroup.hpp"

class LayerGroup: public TLayerGroup<ILayer>{
    Q_OBJECT

    public:
        LayerGroup(QObject *parent=nullptr);
        LayerGroup(std::initializer_list<ILayer*> layers, QObject *parent=nullptr);
        ~LayerGroup();
};