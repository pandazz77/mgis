#include "MapGraphicsView.h"
#include "SphericalMercator.h"

MapGraphicsView::MapGraphicsView(QWidget *parent) : QGraphicsView(new MapGraphicsScene,parent), cam(new MapCamera({0,0},1,this)){
    scale(1,-1);

    setRenderHint(QPainter::RenderHint::Antialiasing);


    setProjection(std::make_unique<SphericalMercator>());

    connect(cam,&MapCamera::projectedPosChanged,this,&MapGraphicsView::onPosChanged);
    connect(cam,&MapCamera::scaleChanged,this,&MapGraphicsView::onScaleChanged);

    connect(scene(),&MapGraphicsScene::mouseMoved,this,&MapGraphicsView::onMouseMove);
    connect(scene(),&MapGraphicsScene::clicked,this,&MapGraphicsView::onMouseClick);
    connect(scene(),&MapGraphicsScene::doubleClicked,this,&MapGraphicsView::onMouseDoubleClick);

    setDragMode(QGraphicsView::ScrollHandDrag);
    setMouseTracking(true); // for mouse tracking in MapGraphicsScene without click
}

MapGraphicsScene *MapGraphicsView::scene(){
    return dynamic_cast<MapGraphicsScene*>(QGraphicsView::scene());
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
    MapPane::addLayer(layer);
    layer->rebuildItem(this);
    QGraphicsItem *item = layer->getItem();
    scene()->addItem(item);
    if(!item){ // if not specified
        item->setZValue(this->getLayers().size()*0.1);
    }
}

MapGraphicsView *MapGraphicsView::map(){
    return this;
}

void MapGraphicsView::removeLayer(ILayer *layer){
    MapPane::removeLayer(layer);
    layer->rebuildItem(nullptr);
    QGraphicsItem *item = layer->getItem();
    if(item)
        scene()->removeItem(item);
}

void MapGraphicsView::setProjection(std::unique_ptr<Projection> _proj){
    this->proj = std::move(_proj);

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

    rebuildAll();
}

Projection *MapGraphicsView::getProjection(){
    return proj.get();
}

MapCamera *MapGraphicsView::getCam(){
    return this->cam;
}

void MapGraphicsView::wheelEvent(QWheelEvent *event){
    double zoom = 0.25;
    int sign = event->angleDelta().y() > 0 ? -1 : 1;
    cam->zoomBy(1+sign*zoom);
}

void MapGraphicsView::rebuildAll(){
    for(ILayer *layer: getLayers()){
        removeLayer(layer);
        addLayer(layer);
    }
}

MapGraphicsView::~MapGraphicsView(){

}