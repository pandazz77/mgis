#include "GeoJsonProvider.h"

#include <QJsonDocument>


int main(int argc, char *argv[]){
    QJsonDocument doc = QJsonDocument::fromJson(R"({
  'type': 'FeatureCollection',
  'features': [
    {
      'type': 'Feature',
      'properties': {},
      'geometry': {
        'coordinates': [
          54.804687499998636,
          24.28702686537683
        ],
        'type': 'Point'
      }
    }
  ]
    })");

    QVariantMap map = doc.toVariant().toMap();

    FeatureCollection *f = dynamic_cast<FeatureCollection*>(ifeatureFromVariant(map));
    qDebug() << f->size();
    

    // ifeatureFromVariant(QVariantMap map)

    return 0;
}