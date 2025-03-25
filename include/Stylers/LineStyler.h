#pragma once

#include "IStyler.h"

#include <QPen>

class LineStyler: public IStyler{
    public: 
        LineStyler();
        LineStyler(const QPen &pen);

        void apply(QGraphicsItem *item,const Geometry::Type &type) override;

        void setPen(const QPen &pen);
        QPen getPen();

        bool isCompatibilityWith(const Geometry::Type &type) override;

    protected:
        QPen pen;
};