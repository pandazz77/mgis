#pragma once

#include "PointStyler.h"
#include "LineStyler.h"
#include "PolyStyler.h"

class FeatureLayer;

namespace RandomStyler {
    IStyler *create(FeatureLayer *layer);

    PointStyler *createPointStyler();
    LineStyler *createLineStyler();
    PolyStyler *createPolyStyler();
};