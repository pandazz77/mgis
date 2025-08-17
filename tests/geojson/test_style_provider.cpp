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
        QPixmap pix;

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
        
        if(point){
            int size = 20;
            QString sym = "circle";
            brush.setStyle(Qt::SolidPattern);
            if(prop.has("marker-color")) brush.setColor(qstr(prop["marker-color"].to<std::string>()));
            if(prop.has("marker-symbol")) sym = qstr(prop["marker-symbol"].to<std::string>());
            if(prop.has("marker-size")) size = prop["marker-size"].to<int>();
            pix = createMarker(sym,brush,size);
        }
        
        if(poly) poly->setBrush(brush);
        if(line) line->setPen(pen);
        if(point) point->setPixmap(pix);
    });

    geojsonProvider->setStyleProvider(styler);
    // ============================

    geojsonProvider->fromFile(filename);
    geojsonProvider->addTo(map);

    map->show();
    return app.exec(); 
}