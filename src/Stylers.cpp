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
    pen.setCosmetic(true);
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

PolyStyler::PolyStyler() : LineStyler(){

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
    polyStyler->setBrush(QBrush(Qt::white)); // init brush
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
    QPen pen = lineStyler->getPen();
    pen.setColor(_randomColor());
    lineStyler->setPen(pen);
}

void RandomStyler::reloadPolyStyler(){
    QPen pen = polyStyler->getPen();
    QBrush brush = polyStyler->getBrush();

    pen.setColor(_randomColor());
    brush.setColor(_randomColor());
    
    polyStyler->setPen(pen);
    polyStyler->setBrush(brush);
}

RandomStyler *RandomStyler::getInstance(){
    if(!instance) instance = new RandomStyler();
    return instance;
}