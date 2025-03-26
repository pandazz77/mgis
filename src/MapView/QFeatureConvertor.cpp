#include "QFeatureConvertor.h"

Point2D QFeatureConvertor::qPoint(QPointF point){
    return Point2D(
        point.x(),
        point.y()
    );
}

QPointF QFeatureConvertor::qPoint(Point2D point){
    return QPointF(
        point.x,
        point.y
    );
}

QVector<QPointF> QFeatureConvertor::qLineString(std::vector<Point2D> line){
    QVector<QPointF> result;
    for(Point2D point: line) result.push_back(QPointF(point.x,point.y));
    return result;
}

QPainterPath QFeatureConvertor::buildLine(LineString<Point2D> line){
    QPainterPath path(QPointF(
        line[0].x,
        line[0].y
    ));

    for(int i = 1; i < line.size(); i++){
        path.lineTo(
            line[i].x,
            line[i].y
        );
    }

    return path;
}

bool QFeatureConvertor::isClockwise(const QPolygonF &polygon) {
    double sum = 0.0;
    int n = polygon.size();

    for (int i = 0; i < n; ++i) {
        const QPointF &p1 = polygon[i];
        const QPointF &p2 = polygon[(i + 1) % n];
        sum += (p2.x() - p1.x()) * (p2.y() + p1.y());
    }

    return sum > 0;
}

QPolygonF QFeatureConvertor::reversePolygon(const QPolygonF &polygon) {
    QPolygonF reversed = polygon;
    std::reverse(reversed.begin(), reversed.end());
    return reversed;
}

QPolygonF QFeatureConvertor::ensurePolygonOrder(const QPolygonF &polygon, bool clockwise) {
    if ((isClockwise(polygon) && !clockwise) || (!isClockwise(polygon) && clockwise)) {
        return reversePolygon(polygon);
    }
    return polygon;
}

QPainterPath QFeatureConvertor::buildPoly(Polygon<Point2D> poly){
    QPainterPath path;

    QPolygonF exterior = ensurePolygonOrder(qLineString(poly.exterior),true);
    path.addPolygon(exterior);
    for(auto interiorRaw: poly.interiors){
        QPolygonF interior = ensurePolygonOrder(qLineString(interiorRaw),false);
        path.addPolygon(interior);
    }
    path.setFillRule(Qt::OddEvenFill);

    return path;
}