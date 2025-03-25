#pragma once

#include "PointStyler.h"
#include "LineStyler.h"
#include "PolyStyler.h"

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