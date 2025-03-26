#pragma once

#include "GeoTypes.hpp"

#include <QPointF>
#include <QPainterPath>

namespace QFeatureConvertor{

    Point2D qPoint(QPointF);
    QPointF qPoint(Point2D);
    
    QVector<QPointF> qLineString(std::vector<Point2D> line);
    
    QPainterPath buildLine(LineString<Point2D> line);
    QPainterPath buildPoly(Polygon<Point2D> poly);
    
    bool isClockwise(const QPolygonF &poly);
    QPolygonF reversePolygon(const QPolygonF &poly);
    QPolygonF ensurePolygonOrder(const QPolygonF &poly, bool clockwise);
    
};