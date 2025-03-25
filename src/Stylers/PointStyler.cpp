#include "PointStyler.h"

PointStyler::PointStyler(){

}

PointStyler::PointStyler(const QPixmap &pixmap, const QPointF &anchor) : pixmap(pixmap), anchor(anchor){

}

void PointStyler::apply(QGraphicsItem *item,const Geometry::Type &type){
    if(dynamic_cast<QGraphicsItemGroup*>(item)) return applyCollection(item,type);
    QGraphicsPixmapItem *pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);

    if(!scalable) pixItem->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    pixItem->setPixmap(this->pixmap);
    pixItem->setOffset(-anchor.x(),-anchor.y());
}

bool PointStyler::isCompatibilityWith(const Geometry::Type &type){
    return type == Geometry::Type::POINT || type == Geometry::Type::MULTIPOINT;
}

void PointStyler::setPixmap(const QPixmap &pixmap){
    this->pixmap = pixmap;
}

void PointStyler::setAnchor(const QPointF &anchor){
    this->anchor = anchor;
}