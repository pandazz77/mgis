#pragma once

#include "LineStyler.h"

class PolyStyler: public LineStyler, public QBrush{
    public:
        PolyStyler();
        PolyStyler(const QPen &pen, const QBrush &brush);

        void setColor(const QColor &color) = delete;
        void setFill(const QColor &color);

        void setStyle(Qt::BrushStyle) = delete;
        Qt::BrushStyle style() = delete;
        void setFillStyle(Qt::BrushStyle style);
        Qt::BrushStyle fillStyle();

        QColor color() = delete;
        QColor fill();

        void setFillOpacity(double opacity);
        double fillOpacity();

        void setTextureSize(QSize size);

        void apply(QGraphicsItem *item,const Geometry::Type &type) override;
        bool isCompatibilityWith(const Geometry::Type &type) override;
};