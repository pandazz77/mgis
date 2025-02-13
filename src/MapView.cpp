#include "MapView.h"

ILayer::ILayer(QObject *parent): QObject(parent){

}

QGraphicsItem *ILayer::getItem(){
    return item;
}

void ILayer::addTo(MapGraphicsView *map){
    // STRONG ORDER / DO NOT CHANGE
    this->map = map;
    onMapChanged();
    map->addLayer(this);
}

void ILayer::removeFrom(MapGraphicsView *map){
    // STRONG ORDER / DO NOT CHANGE
    map->removeLayer(this);
    this->map = nullptr;
    onMapChanged();
}
// =====

QVector<QPointF> qlineString(std::vector<Point2D> line){
    QVector<QPointF> result;
    for(Point2D point: line) result.push_back(QPointF(point.x,point.y));
    return result;
}

QPainterPath buildLineGeometry(LineString<Point2D> line){
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

QPolygonF buildPolyGeometry(Polygon<Point2D> poly){
    QPolygonF exterior = qlineString(poly.exterior);
    for(auto interior: poly.interiors){
        exterior = exterior.subtracted(QPolygonF(qlineString(interior)));
    }

    return exterior;
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

QGraphicsItem *FeatureLayer::buildFeature(IFeature *ifeature,Projection *proj){
    QGraphicsItem *item = nullptr;

    if(dynamic_cast<Feature*>(ifeature)){
        Feature* feature = dynamic_cast<Feature*>(ifeature);
        
        if(dynamic_cast<Point<LatLng>*>(feature->geometry)){
            Point<LatLng> *geometry = dynamic_cast<Point<LatLng>*>(feature->geometry);
            Point<Point2D> projected = proj->project(*geometry);

            item = new QGraphicsRectItem(
                projected.coordinates.x,
                projected.coordinates.y,
                10, 10 // test width and height
            );

        } else if(dynamic_cast<LineString<LatLng>*>(feature->geometry)){
            LineString<LatLng> *geometry = dynamic_cast<LineString<LatLng>*>(feature->geometry);
            LineString<Point2D> projected = proj->project(*geometry);

            item = new QGraphicsPathItem(
                buildLineGeometry(projected)
            );
            
        } else if(dynamic_cast<Polygon<LatLng>*>(feature->geometry)){
            Polygon<LatLng> *geometry = dynamic_cast<Polygon<LatLng>*>(feature->geometry);
            Polygon<Point2D> projected = proj->project(*geometry);

            item = new QGraphicsPolygonItem(
                buildPolyGeometry(projected)
            );
        }

        feature->styler->apply(item,feature->geometry->type());

    } else if(dynamic_cast<FeatureCollection*>(ifeature)){
        FeatureCollection *collection = dynamic_cast<FeatureCollection*>(ifeature);

        QGraphicsItemGroup *group = new QGraphicsItemGroup;

        for(IFeature *feature: *collection){
            QGraphicsItem *subItem = buildFeature(feature,proj);
            group->addToGroup(subItem);
        }

        item = group;
    }

    return item;
}

// =====

MapCamera::MapCamera(LatLng pos, double scale, MapGraphicsView *map) : QObject(map), map(map), pos(pos), scale(scale){

}

MapCamera::~MapCamera(){

}


Point2D MapCamera::getProjectedPos(){
    return map->getProjection()->project(pos);
}

LatLng MapCamera::getPos(){
    return pos;
}

double MapCamera::getScale(){
    return scale;
}

void MapCamera::setPos(LatLng pos){
    this->pos = pos;
    emit posChanged(this->pos);
    emit projectedPosChanged(this->map->getProjection()->project(pos));
}

void MapCamera::setProjectedPos(Point2D pos){
    setPos(map->getProjection()->unproject(pos));
}

void MapCamera::setScale(double scale){
    this->scale = scale;
    emit scaleChanged(this->scale);
}

void MapCamera::zoomBy(double zoom){
    setScale(this->scale*zoom);
}

// =====

Point2D _mgisConvert(QPointF point){
    return Point2D(point.x(),point.y());
}

MapGraphicsScene::MapGraphicsScene(QObject *parent) : QGraphicsScene(parent){

}

MapGraphicsScene::~MapGraphicsScene(){

}

void MapGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
    emit mouseMoved(_mgisConvert(mouseEvent->scenePos()));
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void MapGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if(mouseEvent->lastScreenPos()==mouseEvent->screenPos())
        emit clicked(_mgisConvert(mouseEvent->scenePos()));
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void MapGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if(mouseEvent->lastScreenPos()==mouseEvent->screenPos())
        emit doubleClicked(_mgisConvert(mouseEvent->scenePos()));
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}



// =====

MapGraphicsView::MapGraphicsView(QWidget *parent) : QGraphicsView(new MapGraphicsScene,parent), proj(new SphericalMercator), cam(new MapCamera({0,0},1,this)){
    scale(1,-1);

    setRenderHint(QPainter::RenderHint::Antialiasing);

    Bounds<Point2D> bounds = proj->bounds();
    setSceneRect(
        QRectF{
            QPointF{bounds.SW.x,bounds.NE.y},
            QPointF{bounds.NE.x,bounds.SW.y}
        }
    );
    fitInView(sceneRect(),Qt::KeepAspectRatio);

    const double widthM = this->widthMM() * 0.001,
                 left = sceneRect().left(),
                 right = sceneRect().right(),
                 initScale = right-left/widthM;

    previousScale = initScale;
    cam->setScale(initScale);
    qDebug() << "initial scale:" << cam->getScale();

    connect(cam,&MapCamera::projectedPosChanged,this,&MapGraphicsView::onPosChanged);
    connect(cam,&MapCamera::scaleChanged,this,&MapGraphicsView::onScaleChanged);

    connect(scene(),&MapGraphicsScene::mouseMoved,this,&MapGraphicsView::onMouseMove);
    connect(scene(),&MapGraphicsScene::clicked,this,&MapGraphicsView::onMouseClick);
    connect(scene(),&MapGraphicsScene::doubleClicked,this,&MapGraphicsView::onMouseDoubleClick);

    setDragMode(QGraphicsView::ScrollHandDrag);
    setMouseTracking(true); // for mouse tracking in MapGraphicsScene without click
}

void MapGraphicsView::onPosChanged(Point2D pos){
    auto previousRect = scene()->sceneRect();
    scene()->setSceneRect(pos.x,pos.y,previousRect.width(),previousRect.height());
}

void MapGraphicsView::onScaleChanged(double scale){
    double factor = previousScale / scale;

    this->scale(factor,factor);
    previousScale = scale;
}

void MapGraphicsView::onMouseMove(Point2D pos){
    emit cam->mouseMoved(proj->unproject(pos));
}

void MapGraphicsView::onMouseClick(Point2D pos){
    emit cam->clicked(proj->unproject(pos));
}

void MapGraphicsView::onMouseDoubleClick(Point2D pos){
    emit cam->dblClicked(proj->unproject(pos));
}

void MapGraphicsView::addLayer(ILayer *layer){
    scene()->addItem(layer->getItem());
    layerSet.insert(layer);
    if(!layer->getItem()->zValue()){ // if not specified
        layer->getItem()->setZValue(layerSet.size());
    }
}

void MapGraphicsView::removeLayer(ILayer *layer){
    scene()->removeItem(layer->getItem());
    layerSet.remove(layer);
}

void MapGraphicsView::setProjection(Projection *proj){
    this->proj = proj;
}

Projection *MapGraphicsView::getProjection(){
    return proj;
}

MapCamera *MapGraphicsView::getCam(){
    return this->cam;
}

void MapGraphicsView::wheelEvent(QWheelEvent *event){
    double zoom = 0.25;
    int sign = event->angleDelta().y() > 0 ? -1 : 1;
    cam->zoomBy(1+sign*zoom);
}

MapGraphicsView::~MapGraphicsView(){

}