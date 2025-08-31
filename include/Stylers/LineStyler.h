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

        void setStyle(Qt::PenStyle) = delete;
        Qt::PenStyle style() = delete;
        void setStrokeStyle(Qt::PenStyle style);
        Qt::PenStyle strokeStyle();

        QColor color() = delete;
        QColor stroke();

        void setStrokeOpacity(double opacity);
        double strokeOpacity();

        bool isCompatibilityWith(const Geometry::Type &type) override;
};