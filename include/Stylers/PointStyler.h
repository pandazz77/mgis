#pragma once

#include "IStyler.h"

#include "TextPixmap.h"

class PointStyler: public IStyler{
    public:
        PointStyler();
        PointStyler(const QPixmap &pixmap, const QPointF &anchor=QPointF());

        void apply(QGraphicsItem *item,const Geometry::Type &type) override;
        bool isCompatibilityWith(const Geometry::Type &type) override;

        void setPixmap(const QPixmap &pixmap);
        void setAnchor(const QPointF &anchor);

        void setText(const TextPixmap &text);
        TextPixmap &text();

    protected:
        TextPixmap _text;
        QPixmap pixmap;
        QPointF anchor; // icon anchor
        bool scalable = false;
};