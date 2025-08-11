#include <QApplication>
#include "MapGraphicsView.h"
#include "GeoJsonProvider.h"
#include "FeatureCollectionLayer.h"
#include <QFile>

int main(int argc, char *argv[]){
    QApplication app(argc,argv);

    MapGraphicsView *map = new MapGraphicsView();

    QFile file("world.json");
    file.open(QIODevice::ReadOnly);
    QJsonDocument json = QJsonDocument::fromJson(file.readAll());
    FeatureCollection collection = GeoJsonProvider::transformCollection(json.toVariant().toMap());

    FeatureCollectionLayer *layer = new FeatureCollectionLayer(&collection,map);
    layer->addTo(map);

    map->show();
    return app.exec(); 
}