#include "LineStyler.h"

LineStyler::LineStyler(){
    pen.setCosmetic(true);
}

LineStyler::LineStyler(const QPen &pen) : pen(pen){

}

void LineStyler::apply(QGraphicsItem *item,const Geometry::Type &type){
    if(dynamic_cast<QGraphicsItemGroup*>(item)) return applyCollection(item,type);
    QGraphicsPathItem *lineItem = dynamic_cast<QGraphicsPathItem*>(item);
    lineItem->setPen(pen);
}

bool LineStyler::isCompatibilityWith(const Geometry::Type &type){
    return type == Geometry::Type::LINESTRING || type == Geometry::Type::MULTILINESTRING;
}

void LineStyler::setPen(const QPen &pen){
    this->pen = pen;
}

QPen LineStyler::getPen(){
    return pen;
}