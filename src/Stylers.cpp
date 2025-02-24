#include "Stylers.h"
#include "MapView.h"

#include <QRandomGenerator>

// =====================

PointStyler::PointStyler(){

}

PointStyler::PointStyler(const QPixmap &pixmap, const QPointF &anchor) : pixmap(pixmap), anchor(anchor){

}

void PointStyler::apply(QGraphicsItem *item,const Geometry::Type &type){
    QGraphicsPixmapItem *pixItem = dynamic_cast<QGraphicsPixmapItem*>(item);

    if(!scalable) pixItem->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    pixItem->setPixmap(this->pixmap);
    pixItem->setOffset(-anchor.x(),-anchor.y());
}

bool PointStyler::isCompatibilityWith(const Geometry::Type &type){
    return type == Geometry::Type::POINT;
}

void PointStyler::setPixmap(const QPixmap &pixmap){
    this->pixmap = pixmap;
}

void PointStyler::setAnchor(const QPointF &anchor){
    this->anchor = anchor;
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
    reloadPointStyler(); // init point styler
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
    QPixmap pixmap(12,12);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    QPen pen(_randomColor(),1);
    QBrush brush(_randomColor());

    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawEllipse(pixmap.rect());

    pointStyler->setPixmap(pixmap);
    pointStyler->setAnchor(QPointF(pixmap.height()/2,pixmap.width()/2));
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