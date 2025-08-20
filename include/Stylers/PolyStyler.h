#pragma once

#include "LineStyler.h"

class PolyStyler: public LineStyler, public QBrush{
    public:
        PolyStyler();
        PolyStyler(const QPen &pen, const QBrush &brush);

        void setColor(const QColor &color) = delete;
        void setFill(const QColor &color);

        QColor color() = delete;
        QColor fill();

        void apply(QGraphicsItem *item,const Geometry::Type &type) override;
        bool isCompatibilityWith(const Geometry::Type &type) override;
};