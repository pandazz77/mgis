#include "PointStyler.h"

#include <QPainter>

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

QPixmap PointStyler::TextPixmap(const QString &text, const QFont &font,const QColor& textColor, const QColor &bgColor){
    QFontMetrics fm(font);
    QSize textSize = fm.size(Qt::TextSingleLine,text);
    QSize pixSize(textSize*1.5);

    QPixmap pix(pixSize);
    pix.fill(bgColor);
    QPainter painter(&pix);
    QPen pen(textColor);
    pen.setCosmetic(true);
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(10,10,text);
    return pix;
}