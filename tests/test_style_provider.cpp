#include <QApplication>
#include "MapGraphicsView.h"
#include "GeoJsonProvider.h"
#include "FeatureCollectionLayer.h"
#include <QFile>
#include <QCommandLineParser>

#include "SimpleStyleProvider.h"

#include "TestDefines.h"

QString qstr(const std::string &s){
    return QString::fromStdString(s);
}

QPolygon triangle(QRect rect){
    return QPolygon({
        QPoint{rect.width()/2,0}, // top
        rect.bottomLeft(),
        rect.bottomRight()
    });
}

QPixmap createMarker(QString sym, QBrush brush, int size){
    QPixmap pixmap(size,size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    QPen pen(Qt::black,1);
    pen.setCosmetic(true);

    painter.setPen(pen);
    qDebug() << brush;
    painter.setBrush(brush);

    if(sym=="triangle") 
        painter.drawPolygon(triangle(pixmap.rect()));
    else if(sym=="circle")
        painter.drawEllipse(pixmap.rect());
    else if(sym=="rect")
        painter.drawRect(pixmap.rect());

    return pixmap;
}

int main(int argc, char *argv[]){
    QApplication app(argc,argv);

    QCommandLineParser parser;
    QCommandLineOption input_file_option(QStringList() << "i" << "input","geojson input file","input");
    parser.addOption(input_file_option);

    parser.process(app);

    QString filename =  qpath({DATASETS_DIR,"style_properties.json"});
    if(parser.isSet(input_file_option)){
        filename = parser.value(input_file_option);
    }

    MapGraphicsView *map = new MapGraphicsView();

    GeoJsonProvider *geojsonProvider = new GeoJsonProvider(map);

    // ============================
    SimpleStyleProvider *styler = new SimpleStyleProvider([](const Feature *feature, IStyler *& styler){
        const Feature::Properties &prop = feature->properties;

        PointStyler *point = dynamic_cast<PointStyler*>(styler);
        LineStyler *line = dynamic_cast<LineStyler*>(styler);
        PolyStyler *poly = dynamic_cast<PolyStyler*>(styler);

        QPixmap pix;

        if(prop.has("stroke")) 
            line->setStroke(qstr(prop["stroke"].to<std::string>()));
        if(prop.has("stroke-width")) 
            line->setWidth(prop["stroke-width"].to<int>());
        if(prop.has("stroke-opacity"))
            line->setStrokeOpacity(prop["stroke-opacity"].to<double>());
        if(prop.has("fill"))
            poly->setFill(qstr(prop["fill"].to<std::string>()));
        if(prop.has("fill-opacity"))
            poly->setFillOpacity(prop["fill-opacity"].to<double>());
        
        if(point){
            int size = 20;
            QString sym = "circle";
            QBrush brush;
            brush.setStyle(Qt::SolidPattern);
            if(prop.has("marker-color")) brush.setColor(qstr(prop["marker-color"].to<std::string>()));
            if(prop.has("marker-symbol")) sym = qstr(prop["marker-symbol"].to<std::string>());
            if(prop.has("marker-size")) size = prop["marker-size"].to<int>();
            pix = createMarker(sym,brush,size);
            point->setPixmap(pix);
        }
        
    });

    geojsonProvider->setStyleProvider(styler);
    // ============================

    geojsonProvider->fromFile(filename);
    geojsonProvider->addTo(map);

    map->show();
    return app.exec(); 
}