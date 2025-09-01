#pragma once

#include "GeoTypes.hpp"
#include <QGraphicsItem>

class IStyler{
    public:
        virtual void apply(QGraphicsItem *item,const Geometry::Type &type);
        virtual bool isCompatibilityWith(const Geometry::Type &type) = 0;

        void setZValue(double zValue);
        double getZValue();

    protected:
        void applyCollection(QGraphicsItem *item, const Geometry::Type &type);
    
    private:
        double zValue = std::nan("");
};