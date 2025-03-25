#pragma once

#include "GeoTypes.hpp"
#include <QGraphicsItem>

class IStyler{
    public:
        virtual void apply(QGraphicsItem *item,const Geometry::Type &type) = 0;
        virtual bool isCompatibilityWith(const Geometry::Type &type) = 0;

    protected:
        void applyCollection(QGraphicsItem *item, const Geometry::Type &type);
};