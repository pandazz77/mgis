#include "RandomStyler.h"

#include <QPainter>
#include <QRandomGenerator>

#include "FeatureLayer.h"

#include "PointStyler.h"
#include "LineStyler.h"
#include "PolyStyler.h"

QColor _randomColor(){
    return QColor(
        QRandomGenerator::global()->bounded(255),
        QRandomGenerator::global()->bounded(255),
        QRandomGenerator::global()->bounded(255)
    );
}

IStyler *RandomStyler::create(FeatureLayer *layer){
    using T = Geometry::Type;

    switch(layer->getFeature()->geometry->type()){
        case T::POINT:
        case T::MULTIPOINT:
            return createPointStyler();
        case T::LINESTRING:
        case T::LINEARRING:
        case T::MULTILINESTRING:
            return createLineStyler();
        case T::POLYGON:
        case T::MULTIPOLYGON:
            return createPolyStyler();
        default:
            abort();
    }
}

PointStyler *RandomStyler::createPointStyler(){
    QPixmap pixmap(12,12);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    QPen pen(_randomColor(),1);
    QBrush brush(_randomColor());

    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawEllipse(pixmap.rect());

    const QPointF anchor(pixmap.height()/2,pixmap.width()/2);

    return new PointStyler(pixmap, anchor);
}

LineStyler *RandomStyler::createLineStyler(){
    QPen pen(_randomColor(),2);
    pen.setCosmetic(true);

    return new LineStyler(pen);
}

PolyStyler *RandomStyler::createPolyStyler(){
    QPen pen(_randomColor(),1.5);
    pen.setCosmetic(true);
    QBrush brush(_randomColor());

    return new PolyStyler(pen,brush);
}