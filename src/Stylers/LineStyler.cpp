#include "LineStyler.h"

LineStyler::LineStyler(){
    setCosmetic(true);
}

LineStyler::LineStyler(const QPen &pen) : QPen(pen){
    
}

void LineStyler::setStroke(const QColor &color){
    QPen::setColor(color);
}

QColor LineStyler::stroke(){
    return QPen::color();
}

void LineStyler::apply(QGraphicsItem *item,const Geometry::Type &type){
    if(dynamic_cast<QGraphicsItemGroup*>(item)) return applyCollection(item,type);
    QGraphicsPathItem *lineItem = dynamic_cast<QGraphicsPathItem*>(item);
    lineItem->setPen(*this);
}

bool LineStyler::isCompatibilityWith(const Geometry::Type &type){
    return type == Geometry::Type::LINESTRING || type == Geometry::Type::LINEARRING || type == Geometry::Type::MULTILINESTRING;
}