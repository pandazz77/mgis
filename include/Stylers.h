#pragma once

#include <QGraphicsItem>
#include <QPen>
#include <QBrush>

#include "GeoTypes.hpp"

/// TODO: REIMPLEMENT isCompabilityWith(QGraphicsItem *item) to isCompabilityWith(Geometry *geometry)

class IStyler{
    public:
        virtual void apply(QGraphicsItem *item) = 0;
        virtual bool isCompabilityWith(QGraphicsItem *item) = 0;
};

class PointStyler: public IStyler{
    public:
        PointStyler();

        void apply(QGraphicsItem *item) override;
        bool isCompabilityWith(QGraphicsItem *item) override;
};

class LineStyler: public IStyler{
    public: 
        LineStyler();
        LineStyler(const QPen &pen);

        void apply(QGraphicsItem *item) override;

        void setPen(const QPen &pen);
        QPen getPen();

        bool isCompabilityWith(QGraphicsItem *item) override;

    protected:
        QPen pen;
};

class PolyStyler: public LineStyler{
    public:
        PolyStyler();
        PolyStyler(const QPen &pen, const QBrush &brush);

        void apply(QGraphicsItem *item) override;

        void setBrush(const QBrush &brush);
        QBrush getBrush();

        bool isCompabilityWith(QGraphicsItem *item) override;

    protected:
        QBrush brush;
};

class RandomStyler: public IStyler{
    public:
        static RandomStyler *getInstance();

        void apply(QGraphicsItem *item) override;

        bool isCompabilityWith(QGraphicsItem *item) override;

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