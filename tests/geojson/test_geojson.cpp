#include "GeoJsonProvider.h"

#include <QFile>
#include <QJsonDocument>


int main(int argc, char *argv[]){
    QFile file("all_types.json");
    bool opened = file.open(QIODevice::ReadOnly);
    assert(opened);

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

    QVariantMap map = doc.toVariant().toMap();

    FeatureCollection f = collectionFromVariant(map);
    qDebug() << f.size();

    return 0;
}