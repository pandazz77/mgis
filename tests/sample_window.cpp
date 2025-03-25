#include <iostream>
#include "GeoTypes.hpp"
#include "Projection.h"

#include <QApplication>
#include "MapGraphicsView.h"

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
        {-78,-180},
        {-90,-180},
        {-90,180},
        {-78,180},
        {-77.64258717872744,164.63400047238483},
        {-68.41986844296314,153.80781942719483},
        {-66.26929181149742,97.78254596987438},
        {-70.10097045322112,0},
        {-77.90565519438996,-44.38841054027045},
        {-64.8410226606347,-62.83500118096127},
        {-73.62049837449766,-78.93762075812214},
        {-78,-180}
    }))),map);

    eurasia->addTo(map);
    africa->addTo(map);
    northAmerica->addTo(map);
    southAmerica->addTo(map);
    australia->addTo(map);
    greenland->addTo(map);
    antarctica->addTo(map);


    FeatureLayer *pointTest = new FeatureLayer(new Feature(new Point(
        -62.7798,-154.177
    )),map);
    pointTest->addTo(map);

    FeatureLayer *lineTest = new FeatureLayer(new Feature(new LineString({
        {-48.3894, -112.405},
        {-69.76, -113.924},
        {-57.4991, -64.557},
    })),map);
    lineTest->addTo(map);

    FeatureLayer *ringTest = new FeatureLayer(new Feature(new LinearRing({
        {-54.529, -37.2152},
        {-66.0551, -42.5316},
        {-65.745, -6.07595},
        {-53.1853, -20.5063}, // will be closed at LinearRing constructor
    })),map);
    ringTest->addTo(map);

    FeatureLayer *polyTest = new FeatureLayer(new Feature(new Polygon(
        LinearRing({
            {-53.638, 12.1519},
            {-64.4664, 5.31646},
            {-64.1371, 30.3797},
            {-57.9049, 18.2278},
            {-52.2654, 27.3418},
        }),
        {
            LinearRing({
                {-59.2272, 16.9158},
                {-63.4983, 25.6086},
                {-63.4983, 10.4157},
            }),
            LinearRing({
                {-54.3452, 13.235},
                {-53.7012, 22.3194},
                {-56.994, 16.4459},
            })
        }
    )),map);
    polyTest->addTo(map);

    FeatureLayer *multiPointTest = new FeatureLayer(new Feature(new MultiPoint({
        {-52.359, 42.5831},
        {-58.7971, 43.3989},
        {-63.6551, 43.3989},
    })),map);
    multiPointTest->addTo(map);

    FeatureLayer *multiLineStringTest = new FeatureLayer(new Feature(new MultiLineString({
        LineString({
            {-52.1734, 49.4735},
            {-52.1734, 64.8833},
            {-56.4281, 72.453},
        }),
        LineString({
            {-57.1734, 49.4735},
            {-57.1734, 64.8833},
            {-61.4281, 72.453},
        }),
        LineString({
            {-62.1734, 49.4735},
            {-62.1734, 64.8833},
            {-66.4281, 72.453},
        })
    })),map);
    multiLineStringTest->addTo(map);


    FeatureLayer *multiPolygonTest = new FeatureLayer(new Feature(new MutliPolygon({
        Polygon(
            LinearRing({
                {-52.5269, 80.5571},
                {-64.6115, 80.5571},
                {-64.392, 89.7345},
                {-52.5269, 94.0683},
            }),
            {
                LinearRing({
                    {-54.0841, 82.8783},
                    {-63.6098, 83.4866},
                    {-63.5082, 87.6685},
                    {-54.0395, 89.4933},
                })
            }
        ),

        Polygon(
            LinearRing({
                {-52.5269, 100.5571},
                {-64.6115, 100.5571},
                {-64.392, 109.7345},
                {-52.5269, 114.0683},
            }),
            {
                LinearRing({
                    {-54.0841, 102.8783},
                    {-63.6098, 103.4866},
                    {-63.5082, 107.6685},
                    {-54.0395, 109.4933},
                })
            }
        ),

        Polygon(
            LinearRing({
                {-52.5269, 120.5571},
                {-64.6115, 120.5571},
                {-64.392, 129.7345},
                {-52.5269, 134.0683},
            }),
            {
                LinearRing({
                    {-54.0841, 122.8783},
                    {-63.6098, 123.4866},
                    {-63.5082, 127.6685},
                    {-54.0395, 129.4933},
                })
            }
        ),
    })),map);
    multiPolygonTest->addTo(map);

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