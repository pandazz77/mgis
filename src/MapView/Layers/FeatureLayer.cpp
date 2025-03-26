#include "FeatureLayer.h"

#include "MapGraphicsView.h"

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

// =====

FeatureLayer::FeatureLayer(IFeature *feature, QObject *parent) : ILayer(parent), feature(feature){

}

FeatureLayer::~FeatureLayer(){
    delete feature;
}

void FeatureLayer::onMapChanged(){
    if(this->map){
        this->item = buildFeature(this->feature,this->map->getProjection());
    } else {
        delete this->item;
    }
}

IFeature *FeatureLayer::getFeature(){
    return feature;
}

FGraphicsItem *FeatureLayer::buildFeatureGeometry(Geometry *_geometry,Projection *proj){
    FGraphicsItem *item = nullptr;

    if(dynamic_cast<Point<LatLng>*>(_geometry)){
        Point<LatLng> *geometry = dynamic_cast<Point<LatLng>*>(_geometry);
        Point<Point2D> projected = proj->project(*geometry);

        item = new FGraphicsPoint();
        item->setPos(projected.coordinates.x,projected.coordinates.y);

    } else if(dynamic_cast<LineString<LatLng>*>(_geometry)){
        LineString<LatLng> *geometry = dynamic_cast<LineString<LatLng>*>(_geometry);
        LineString<Point2D> projected = proj->project(*geometry);

        item = new FGraphicsLineString(
            QFeatureConvertor::buildLine(projected)
        );
        
    } else if(dynamic_cast<Polygon<LatLng>*>(_geometry)){
        Polygon<LatLng> *geometry = dynamic_cast<Polygon<LatLng>*>(_geometry);
        Polygon<Point2D> projected = proj->project(*geometry);

        item = new FGraphicsPolygon(
            QFeatureConvertor::buildPoly(projected)
        );
    }

    return item;
}

template<class GeometryUnit>
FGraphicsCollection *FeatureLayer::buildFeatureGeometryCollection(GeometryCollection<GeometryUnit> *collection, Projection *proj){
    FGraphicsCollection *group = new FGraphicsCollection;
    for(auto gunit: *collection){
        FGraphicsItem *subItem = buildFeatureGeometry(&gunit,proj);
        group->addToGroup(subItem);
    }
    return group;
}

FGraphicsItem *FeatureLayer::buildFeature(IFeature *ifeature,Projection *proj){
    FGraphicsItem *item = nullptr;

    if(dynamic_cast<Feature*>(ifeature)){
        Feature* feature = dynamic_cast<Feature*>(ifeature);
        
        item = buildFeatureGeometry(feature->geometry,proj);
        if(!item) {
            if(dynamic_cast<MultiPoint<LatLng>*>(feature->geometry)){
                MultiPoint<LatLng> *geometry = dynamic_cast<MultiPoint<LatLng>*>(feature->geometry);
                item = buildFeatureGeometryCollection(geometry,proj);
            } else if(dynamic_cast<MultiLineString<LatLng>*>(feature->geometry)){
                MultiLineString<LatLng> *geometry = dynamic_cast<MultiLineString<LatLng>*>(feature->geometry);
                item = buildFeatureGeometryCollection(geometry,proj);
            } else if(dynamic_cast<MutliPolygon<LatLng>*>(feature->geometry)){
                MutliPolygon<LatLng> *geometry = dynamic_cast<MutliPolygon<LatLng>*>(feature->geometry);
                item = buildFeatureGeometryCollection(geometry,proj);
            }
        }

        feature->styler->apply(item,feature->geometry->type());

    } else if(dynamic_cast<FeatureCollection*>(ifeature)){
        FeatureCollection *collection = dynamic_cast<FeatureCollection*>(ifeature);

        FGraphicsCollection *group = new FGraphicsCollection;

        for(IFeature *feature: *collection){
            FGraphicsItem *subItem = buildFeature(feature,proj);
            group->addToGroup(subItem);
        }

        item = group;
    }

    return item;
}