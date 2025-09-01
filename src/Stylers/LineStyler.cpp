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

void LineStyler::setStrokeStyle(Qt::PenStyle style){
    QPen::setStyle(style);
}

Qt::PenStyle LineStyler::strokeStyle(){
    return QPen::style();
}


void LineStyler::setStrokeOpacity(double opacity){
    QColor color = stroke();
    color.setAlphaF(opacity);
    setStroke(color);
}

double LineStyler::strokeOpacity(){
    return stroke().alphaF();
}


void LineStyler::apply(QGraphicsItem *item,const Geometry::Type &type){
    if(dynamic_cast<QGraphicsItemGroup*>(item)) return applyCollection(item,type);
    QGraphicsPathItem *lineItem = dynamic_cast<QGraphicsPathItem*>(item);
    lineItem->setPen(*this);
    IStyler::apply(item,type);
}

bool LineStyler::isCompatibilityWith(const Geometry::Type &type){
    return type == Geometry::Type::LINESTRING || type == Geometry::Type::LINEARRING || type == Geometry::Type::MULTILINESTRING;
}