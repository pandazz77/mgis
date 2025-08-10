#include "FeatureLayer.h"

#include "MapGraphicsView.h"
#include "QFeatureConvertor.h"

FeatureLayer::FeatureLayer(Feature *feature, QObject *parent) : ILayer(parent), feature(feature){

}

FeatureLayer::~FeatureLayer(){
    delete feature;
}

QGraphicsItem *FeatureLayer::getItem(){
    return item;
}

void FeatureLayer::rebuildItem(MapPane *pane){
    MapGraphicsView *map = pane->map();
    if(map){
        this->item = buildFeature(this->feature,map->getProjection());
    } else {
        delete this->item;
    }
}

Feature *FeatureLayer::getFeature(){
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

FGraphicsItem *FeatureLayer::buildFeature(Feature *feature,Projection *proj){
    FGraphicsItem *item = nullptr;
    
    item = buildFeatureGeometry(feature->geometry,proj);
    if(!item) {
        if(dynamic_cast<MultiPoint<LatLng>*>(feature->geometry)){
            MultiPoint<LatLng> *geometry = dynamic_cast<MultiPoint<LatLng>*>(feature->geometry);
            item = buildFeatureGeometryCollection(geometry,proj);
        } else if(dynamic_cast<MultiLineString<LatLng>*>(feature->geometry)){
            MultiLineString<LatLng> *geometry = dynamic_cast<MultiLineString<LatLng>*>(feature->geometry);
            item = buildFeatureGeometryCollection(geometry,proj);
        } else if(dynamic_cast<MultiPolygon<LatLng>*>(feature->geometry)){
            MultiPolygon<LatLng> *geometry = dynamic_cast<MultiPolygon<LatLng>*>(feature->geometry);
            item = buildFeatureGeometryCollection(geometry,proj);
        }
    }

    feature->styler->apply(item,feature->geometry->type());

    return item;
}