#pragma once

#include <QFont>

#include "IStyler.h"

class PointStyler: public IStyler{
    public:
        PointStyler();
        PointStyler(const QPixmap &pixmap, const QPointF &anchor=QPointF());

        void apply(QGraphicsItem *item,const Geometry::Type &type) override;
        bool isCompatibilityWith(const Geometry::Type &type) override;

        void setPixmap(const QPixmap &pixmap);
        void setAnchor(const QPointF &anchor);

        static QPixmap TextPixmap(const QString &text, const QFont &font=QFont(), 
            const QColor& textColor = Qt::black, const QColor &bgColor=Qt::transparent);

    protected:
        QPixmap pixmap;
        QPointF anchor; // icon anchor
        bool scalable = false;
};