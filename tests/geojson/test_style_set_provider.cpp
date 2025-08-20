#include <QApplication>
#include "MapGraphicsView.h"
#include "GeoJsonProvider.h"
#include <QFile>
#include <QCommandLineParser>

#include "StyleSetProvider.h"

QString qstr(const std::string &s){
    return QString::fromStdString(s);
}

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

    GeoJsonProvider *geojsonProvider = new GeoJsonProvider(map);

    // ============================
    StyleSetProvider *styler = new StyleSetProvider();
    styler->add(StyleUnit(
        { // conditions
            StyleCondition([](const Feature *feature){ return feature->properties["sovereignt"].to<std::string>() == "Russia"; })
        },
        { // instructions
            StyleInstruction([](IStyler *& style){ 
                PolyStyler *poly = dynamic_cast<PolyStyler*>(style);
                poly->setFill(Qt::red);
            })
        }
    ));

    geojsonProvider->setStyleProvider(styler);
    // ============================

    geojsonProvider->fromFile(filename);
    geojsonProvider->addTo(map);

    map->show();
    return app.exec(); 
}