#include <QApplication>
#include "MapGraphicsView.h"
#include "GeoJsonProvider.h"
#include "FeatureCollectionLayer.h"
#include <QFile>
#include <QCommandLineParser>

int main(int argc, char *argv[]){
    QApplication app(argc,argv);

    QCommandLineParser parser;
    QCommandLineOption input_file_option(QStringList() << "i" << "input","geojson input file","input");
    parser.addOption(input_file_option);

    parser.process(app);

    QString filename = "world.json";
    if(parser.isSet(input_file_option)){
        filename = parser.value(input_file_option);
    }

    MapGraphicsView *map = new MapGraphicsView();

    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QJsonDocument json = QJsonDocument::fromJson(file.readAll());
    FeatureCollection collection = GeoJsonProvider::transformCollection(json.toVariant().toMap());

    FeatureCollectionLayer *layer = new FeatureCollectionLayer(&collection,map);
    layer->addTo(map);

    map->show();
    return app.exec(); 
}