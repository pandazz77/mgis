#include "PolyStyler.h"

PolyStyler::PolyStyler() : LineStyler(){

}

PolyStyler::PolyStyler(const QPen &pen, const QBrush &brush): LineStyler(pen), brush(brush){

}

void PolyStyler::apply(QGraphicsItem *item,const Geometry::Type &type){
    if(dynamic_cast<QGraphicsItemGroup*>(item)) return applyCollection(item,type);
    QGraphicsPathItem *polyItem = dynamic_cast<QGraphicsPathItem*>(item);
    polyItem->setPen(pen);
    polyItem->setBrush(brush);
}

bool PolyStyler::isCompatibilityWith(const Geometry::Type &type){
    return type == Geometry::Type::POLYGON || type == Geometry::Type::MULTIPOLYGON;
}

void PolyStyler::setBrush(const QBrush &brush){
    this->brush = brush;
}

QBrush PolyStyler::getBrush(){
    return brush;
}