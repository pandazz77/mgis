#pragma once

#include <QGraphicsItem>
#include <QPen>
#include <QBrush>

#include "GeoTypes.hpp"


class IStyler{
    public:
        virtual void apply(QGraphicsItem *item,const Geometry::Type &type) = 0;
        virtual bool isCompatibilityWith(const Geometry::Type &type) = 0;
};

class PointStyler: public IStyler{
    public:
        PointStyler();

        void apply(QGraphicsItem *item,const Geometry::Type &type) override;
        bool isCompatibilityWith(const Geometry::Type &type) override;
};

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

class RandomStyler: public IStyler{
    public:
        static RandomStyler *getInstance();

        void apply(QGraphicsItem *item,const Geometry::Type &type) override;

        bool isCompatibilityWith(const Geometry::Type &type) override;

    private:
        RandomStyler();
        ~RandomStyler();

        void reloadPointStyler();
        void reloadLineStyler();
        void reloadPolyStyler();

        PointStyler *pointStyler;
        LineStyler  *lineStyler;
        PolyStyler  *polyStyler;

        static inline RandomStyler *instance = nullptr;
};