#include <QApplication>
#include "MapGraphicsView.h"
#include "GeoJsonProvider.h"
#include "FeatureCollectionLayer.h"
#include <QFile>
#include <QCommandLineParser>

#include "SimpleStyleProvider.h"

QString qstr(const std::string &s){
    return QString::fromStdString(s);
}

int opacity2alpha(double opacity){
    return 255 * opacity;
}

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
        Feature::Properties &prop = feature->properties;

        PointStyler *point = dynamic_cast<PointStyler*>(layer->styler);
        LineStyler *line = dynamic_cast<LineStyler*>(layer->styler);
        PolyStyler *poly = dynamic_cast<PolyStyler*>(layer->styler);

        QPen pen;
        QBrush brush;

        if(line) pen = line->getPen();
        if(poly) brush = poly->getBrush();

        if(prop.has("stroke")) 
            pen.setColor(qstr(prop["stroke"].to<std::string>()));
        if(prop.has("stroke-width")) 
            pen.setWidth(prop["stroke-width"].to<int>());
        if(prop.has("stroke-opacity")){
            QColor color = pen.color();
            color.setAlpha(opacity2alpha(prop["stroke-opacity"].to<double>()));
            pen.setColor(color);
        }
        if(prop.has("fill"))
            brush.setColor(qstr(prop["fill"].to<std::string>()));
        if(prop.has("fill-opacity")){
            QColor color = brush.color();
            color.setAlpha(opacity2alpha(prop["fill-opacity"].to<double>()));
            brush.setColor(color);
        }
        
        if(poly) poly->setBrush(brush);
        if(line) line->setPen(pen);
        if(point) void();
    });

    geojsonProvider->setStyleProvider(styler);
    // ============================

    geojsonProvider->fromFile(filename);
    geojsonProvider->addTo(map);

    map->show();
    return app.exec(); 
}