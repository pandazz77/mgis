#pragma once

#include "LineStyler.h"

class PolyStyler: public LineStyler{
    public:
        PolyStyler();
        PolyStyler(const QPen &pen, const QBrush &brush);

        void apply(QGraphicsItem *item,const Geometry::Type &type) override;

        void setBrush(const QBrush &brush);
        QBrush getBrush();

        bool isCompatibilityWith(const Geometry::Type &type) override;

    protected:
        QBrush brush;
};