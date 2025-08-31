#include "IStyler.h"

void IStyler::applyCollection(QGraphicsItem *item, const Geometry::Type &type){
    QGraphicsItemGroup *group = dynamic_cast<QGraphicsItemGroup*>(item);
    if(group){
        for(QGraphicsItem *subItem: group->childItems()) 
            apply(subItem,type);
    }
}

void IStyler::apply(QGraphicsItem *item,const Geometry::Type &type){
    if(!std::isnan(zValue))
        item->setZValue(zValue);
}

void IStyler::setZValue(double zValue){
    this->zValue = zValue;
}

double IStyler::getZValue(){
    return zValue;
}
