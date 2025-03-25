#include "IStyler.h"

void IStyler::applyCollection(QGraphicsItem *item, const Geometry::Type &type){
    QGraphicsItemGroup *group = dynamic_cast<QGraphicsItemGroup*>(item);
    if(group){
        for(QGraphicsItem *subItem: group->childItems()) 
            apply(subItem,type);
    }
}