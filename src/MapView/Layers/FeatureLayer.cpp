#include "FeatureLayer.h"

#include "MapGraphicsView.h"
#include "QFeatureConvertor.h"

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
            } else if(dynamic_cast<MultiPolygon<LatLng>*>(feature->geometry)){
                MultiPolygon<LatLng> *geometry = dynamic_cast<MultiPolygon<LatLng>*>(feature->geometry);
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