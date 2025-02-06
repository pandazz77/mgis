#include "Stylers.h"

#include <QRandomGenerator>

// =====================

PointStyler::PointStyler(){

}

void PointStyler::apply(QGraphicsItem *item){

}

bool PointStyler::isCompabilityWith(QGraphicsItem *item){
    return true;
}

// =====================

LineStyler::LineStyler(){

}

LineStyler::LineStyler(const QPen &pen) : pen(pen){

}

void LineStyler::apply(QGraphicsItem *item){
    QGraphicsPathItem *lineItem = dynamic_cast<QGraphicsPathItem*>(item);
    lineItem->setPen(pen);
}

bool LineStyler::isCompabilityWith(QGraphicsItem *item){
    return dynamic_cast<QGraphicsPathItem*>(item);
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

void PolyStyler::apply(QGraphicsItem *item){
    QGraphicsPolygonItem *polyItem = dynamic_cast<QGraphicsPolygonItem*>(item);
    polyItem->setPen(pen);
    polyItem->setBrush(brush);
}

bool PolyStyler::isCompabilityWith(QGraphicsItem *item){
    return dynamic_cast<QGraphicsPolygonItem*>(item);
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

void RandomStyler::apply(QGraphicsItem *item){
    if(polyStyler->isCompabilityWith(item)){
        polyStyler->apply(item);
        reloadPolyStyler();
    } else if(lineStyler->isCompabilityWith(item)){
        lineStyler->isCompabilityWith(item);
        reloadLineStyler();
    } else if(pointStyler->isCompabilityWith(item)){
        pointStyler->isCompabilityWith(item);
        reloadPointStyler();
    }
}

bool RandomStyler::isCompabilityWith(QGraphicsItem *item){
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