#pragma once

#include "IStyler.h"

#include <QPen>

class LineStyler: public IStyler, public QPen{
    public: 
        LineStyler();
        LineStyler(const QPen &pen);

        void apply(QGraphicsItem *item,const Geometry::Type &type) override;

        void setColor(const QColor &color) = delete;
        void setStroke(const QColor &color);

        QColor color() = delete;
        QColor stroke();

        bool isCompatibilityWith(const Geometry::Type &type) override;
};