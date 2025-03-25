#include "RandomStyler.h"

#include <QPainter>
#include <QRandomGenerator>

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