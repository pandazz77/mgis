#include "Stylers.h"
#include "MapView.h"

#include <QRandomGenerator>

// =====================

PointStyler::PointStyler(){

}

void PointStyler::apply(QGraphicsItem *item,const Geometry::Type &type){
    
}

bool PointStyler::isCompatibilityWith(const Geometry::Type &type){
    return type == Geometry::Type::POINT;
}

// =====================

LineStyler::LineStyler(){

}

LineStyler::LineStyler(const QPen &pen) : pen(pen){

}

void LineStyler::apply(QGraphicsItem *item,const Geometry::Type &type){
    QGraphicsPathItem *lineItem = dynamic_cast<QGraphicsPathItem*>(item);
    lineItem->setPen(pen);
}

bool LineStyler::isCompatibilityWith(const Geometry::Type &type){
    return type == Geometry::Type::LINESTRING || type == Geometry::Type::LINEARRING;
}

void LineStyler::setPen(const QPen &pen){
    this->pen = pen;
}

QPen LineStyler::getPen(){
    return pen;
}


// =====================

PolyStyler::PolyStyler(){

}

PolyStyler::PolyStyler(const QPen &pen, const QBrush &brush): LineStyler(pen), brush(brush){

}

void PolyStyler::apply(QGraphicsItem *item,const Geometry::Type &type){
    QGraphicsPolygonItem *polyItem = dynamic_cast<QGraphicsPolygonItem*>(item);
    polyItem->setPen(pen);
    polyItem->setBrush(brush);
}

bool PolyStyler::isCompatibilityWith(const Geometry::Type &type){
    return type == Geometry::Type::POLYGON;
}

void PolyStyler::setBrush(const QBrush &brush){
    this->brush = brush;
}

QBrush PolyStyler::getBrush(){
    return brush;
}

// =====================

QColor _randomColor(){
    return QColor(
        QRandomGenerator::global()->bounded(255),
        QRandomGenerator::global()->bounded(255),
        QRandomGenerator::global()->bounded(255)
    );
}

RandomStyler::RandomStyler() : pointStyler(new PointStyler), lineStyler(new LineStyler), polyStyler(new PolyStyler) {
    reloadPointStyler();
    reloadLineStyler();
    reloadPolyStyler();
}

RandomStyler::~RandomStyler(){
    delete pointStyler;
    delete lineStyler;
    delete polyStyler;
}

void RandomStyler::apply(QGraphicsItem *item,const Geometry::Type &type){
    if(polyStyler->isCompatibilityWith(type)){
        polyStyler->apply(item,type);
        reloadPolyStyler();
    } else if(lineStyler->isCompatibilityWith(type)){
        lineStyler->apply(item,type);
        reloadLineStyler();
    } else if(pointStyler->isCompatibilityWith(type)){
        pointStyler->apply(item,type);
        reloadPointStyler();
    }
}

bool RandomStyler::isCompatibilityWith(const Geometry::Type &type){
    return true;
}

void RandomStyler::reloadPointStyler(){
    
}

void RandomStyler::reloadLineStyler(){
    lineStyler->setPen(QPen(_randomColor()));
}

void RandomStyler::reloadPolyStyler(){
    polyStyler->setPen(QPen(_randomColor()));
    polyStyler->setBrush(QBrush(_randomColor()));
}

RandomStyler *RandomStyler::getInstance(){
    if(!instance) instance = new RandomStyler();
    return instance;
}