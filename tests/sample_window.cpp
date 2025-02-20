#include <iostream>
#include "GeoTypes.hpp"
#include "Projection.h"

#include <QApplication>
#include "MapView.h"

#include <cassert>

int main(int argc, char *argv[]){
    QApplication app(argc,argv);

    MapGraphicsView *map = new MapGraphicsView();
    //map->getCam()->setScale(0.1);
    //map->setFixedSize(800,600);
    map->show();

    FeatureLayer *water = new FeatureLayer(new Feature(new Polygon(LinearRing({
        {90,180},
        {-90,180},
        {-90,-180},
        {90,-180},
        {90,180}
    }))),map);
    Feature *waterFeature = dynamic_cast<Feature*>(water->getFeature());
    waterFeature->styler = new PolyStyler(QPen(Qt::black),QBrush(Qt::blue));
    water->addTo(map);

    FeatureLayer *line = new FeatureLayer(new Feature(new LineString(std::vector<LatLng>{
        {-10,-10},
        {10,10}
    })),map);
    Feature *lineFeature = dynamic_cast<Feature*>(line->getFeature());
    lineFeature->styler = new LineStyler(QPen(
        QBrush(Qt::white),0 /* COSMETIC (SAVE WIDTH) */
    ));
    line->addTo(map);
    line->getItem()->setZValue(100);

    // ====

    FeatureLayer *eurasia = new FeatureLayer(new Feature(new Polygon(LinearRing({
        {76.50781849872428, 103.39301162991251},
        {49.47925216395771, -1.327077670647867},
        {35.59509010109741, -6.069119450296284},
        {36.87005114735018, 31.867214786888724},
        {12.736812452984424, 46.48851027413599},
        {21.046823049962768, 73.75525050711218},
        {10.41418781580127, 106.9495429646488},
        {48.70293254742444, 138.95832497727247},
        {67.40252064879135, 185.19323232884108},
        {71.03847712017372, 129.4742414179766},
        {76.50781849872428, 103.39301162991251}
    }))),map);

    FeatureLayer *africa = new FeatureLayer(new Feature(new Polygon(LinearRing({
        {32.9836001676598, 9.342516333559956},
        {15.03835122749787, -16.343543306200075},
        {2.567746004855138, 9.342516333559956},
        {-36.870051147349294, 21.197620782679962},
        {-30.292519947040205, 35.028575973320045},
        {9.246104233908426, 48.859531163960156},
        {32.9836001676598, 9.342516333559956}
    }))),map);

    FeatureLayer *northAmerica = new FeatureLayer(new Feature(new Polygon(LinearRing({
        {69.57428548514784, -162.9516683269849},
        {57.02025455095847, -157.41928625072848},
        {57.6599967782054, -135.6849280940087},
        {11.577911174801983, -91.82104163226418},
        {53.41651917722706, -53.88470739508014},
        {68.73059262458912, -96.56308341191256},
        {69.57428548514784, -162.9516683269849}
    }))),map);

    FeatureLayer *southAmerica = new FeatureLayer(new Feature(new Polygon(LinearRing({
        {10.802603071948738, -70.87702377215247},
        {-6.113503742736924, -79.5707670348402},
        {-55.25921846226369, -70.87702377215247},
        {-6.113503742736924, -36.89239101800777},
        {10.802603071948738, -70.87702377215247}
    }))),map);

    FeatureLayer *australia = new FeatureLayer(new Feature(new Polygon(LinearRing({
        {-11.96476992059074, 131.84526230780085},
        {-27.174538377546632, 113.66743548581644},
        {-38.1240841178109, 144.4907070535288},
        {-25.759581534957164, 155.16030105773672},
        {-11.96476992059074, 131.84526230780085}
    }))),map);

    FeatureLayer *greenland = new FeatureLayer(new Feature(new Polygon(LinearRing({
        {79.45527732360159, -19.504904492632335},
        {83.43688720357682, -26.617967162103923},
        {81.1788420689013, -64.94947154759201},
        {76.59970818139112, -66.13498199250412},
        {60.88765363672306, -45.58613428069643},
        {69.43592748691304, -23.851776123975725},
        {79.45527732360159, -19.504904492632335}
    }))),map);

    FeatureLayer *antarctica = new FeatureLayer(new Feature(new Polygon(LinearRing({
        {-83.99,180},
        {-78.02709015676866,180},
        {-67.31776269278731,115.15451713455371},
        {-72.46421873750373,-14.180717382534226},
        {-78.59859492076679,-44.28055377923934},
        {-65.62884009693444,-63.563122708012216},
        {-74.84761514554904,-121.69470827000515},
        {-78.45617410123903,-180},
        {-83.77462420163859,-180},
        {-83.99,180},
    }))),map);

    eurasia->addTo(map);
    africa->addTo(map);
    northAmerica->addTo(map);
    southAmerica->addTo(map);
    australia->addTo(map);
    greenland->addTo(map);
    antarctica->addTo(map);

    MapCamera *cam = map->getCam();
    cam->connect(cam,&MapCamera::mouseMoved,[=](LatLng pos){
        qDebug() << "Mouse moved: " << pos.lat << pos.lng;
    });

    cam->connect(cam,&MapCamera::clicked,[=](LatLng pos){
        qDebug() << "Mouse clicked: " << pos.lat << pos.lng;
    });

    cam->connect(cam,&MapCamera::dblClicked,[=](LatLng pos){
        qDebug() << "Mouse dblclicked: " << pos.lat << pos.lng;
    });

    return app.exec();
}