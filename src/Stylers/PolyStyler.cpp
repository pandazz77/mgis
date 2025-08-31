#include "PolyStyler.h"

PolyStyler::PolyStyler() : LineStyler(){

}

PolyStyler::PolyStyler(const QPen &pen, const QBrush &brush): LineStyler(pen), QBrush(brush){

}

void PolyStyler::setFill(const QColor &color){
    QBrush::setColor(color);
}

QColor PolyStyler::fill(){
    return QBrush::color();
}

void PolyStyler::setFillOpacity(double opacity){
    QColor color = fill();
    color.setAlphaF(opacity);
    setFill(color);
}

double PolyStyler::fillOpacity(){
    return fill().alphaF();
}

void PolyStyler::setTextureSize(QSize size){
    QSize prSize = texture().size();
    QTransform transform;
    transform.rotate(180); // mapview is rotated
    transform.scale(
        static_cast<qreal>(size.width()) / prSize.width(),
        static_cast<qreal>(size.height()) / prSize.height()
    );
    QBrush::setTransform(transform);
}

void PolyStyler::apply(QGraphicsItem *item,const Geometry::Type &type){
    if(dynamic_cast<QGraphicsItemGroup*>(item)) return applyCollection(item,type);
    QGraphicsPathItem *polyItem = dynamic_cast<QGraphicsPathItem*>(item);
    polyItem->setPen(*this);
    polyItem->setBrush(*this);
}

bool PolyStyler::isCompatibilityWith(const Geometry::Type &type){
    return type == Geometry::Type::POLYGON || type == Geometry::Type::MULTIPOLYGON;
}