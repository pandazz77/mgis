#include <QApplication>
#include "MapGraphicsView.h"
#include "GeoJsonProvider.h"
#include "FeatureCollectionLayer.h"
#include <QFile>
#include <QCommandLineParser>

#include "SimpleStyleProvider.h"

int main(int argc, char *argv[]){
    QApplication app(argc,argv);

    QCommandLineParser parser;
    QCommandLineOption input_file_option(QStringList() << "i" << "input","geojson input file","input");
    parser.addOption(input_file_option);

    parser.process(app);

    QString filename = "style_properties.json";
    if(parser.isSet(input_file_option)){
        filename = parser.value(input_file_option);
    }

    MapGraphicsView *map = new MapGraphicsView();

    GeoJsonProvider *geojsonProvider = new GeoJsonProvider(map);

    // ============================
    SimpleStyleProvider *styler = new SimpleStyleProvider([](FeatureLayer *layer){
        Feature *feature = layer->getFeature();
        std::string featureName = std::get<std::string>(feature->properties["name"]);
        qDebug() << featureName;
        QPen pen(Qt::red,5);
        pen.setCosmetic(true);
        if(featureName=="line"){
            LineStyler *styler = dynamic_cast<LineStyler*>(layer->styler);
            QPen pen(Qt::red,5);
            pen.setCosmetic(true);
            styler->setPen(pen);
        } else if(featureName=="polygon"){
            PolyStyler *styler = dynamic_cast<PolyStyler*>(layer->styler);
            QPen pen(Qt::blue,2);
            pen.setCosmetic(true);
            QBrush brush(QColor(0,255,0,100));
            styler->setPen(pen);
            styler->setBrush(brush);
        }
    });

    geojsonProvider->setStyleProvider(styler);
    // ============================

    geojsonProvider->fromFile(filename);
    geojsonProvider->addTo(map);

    map->show();
    return app.exec(); 
}