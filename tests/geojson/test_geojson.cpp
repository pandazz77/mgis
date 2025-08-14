#include "GeoJsonProvider.h"

#include <QFile>
#include <QJsonDocument>

QJsonDocument readJson(QString filename){
    QFile file(filename);
    bool opened = file.open(QIODevice::ReadOnly);
    assert(opened);
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    return doc;
}

void writeJson(QString filename, QJsonDocument doc){
    QFile file(filename);
    bool opened = file.open(QIODevice::WriteOnly);
    assert(opened);
    file.write(doc.toJson(QJsonDocument::JsonFormat::Indented));
    file.close();
}

/*
checks:
    - ifeature is feature?
    - feature->geometry is G?
return G type geometry
*/
template<typename G>
G *check_geometry(IFeature *ifeature){
    Feature *feature = dynamic_cast<Feature*>(ifeature);
    assert(feature);
    G *geometry = dynamic_cast<G*>(feature->geometry);
    assert(geometry);
    return geometry;
}

void test_types(){
    QJsonDocument doc = readJson("all_types.json");
    QVariantMap map = doc.toVariant().toMap();
    FeatureCollection collection = GeoJsonProvider::transformCollection(map);

    assert(collection.size()==8);

    Feature *feature;

    // POINT ============================================
    auto point = check_geometry<Point<LatLng>>(collection[0]);
    assert(point->coordinates == LngLat(35.30229398088488,61.61740985783632));
    // ==================================================

    // LINESTRING =======================================
    auto line = check_geometry<LineString<LatLng>>(collection[1]);
    assert(line->at(0)==LngLat(41.981981480885764,65.44962822651894));
    assert(line->at(1)==LngLat(50.59526273088542,53.554070976553675));
    assert(line->at(2)==LngLat( 54.462450230885054,65.44962822651894));
    // ==================================================

    // POLYGON ==========================================
    auto poly = check_geometry<Polygon<LatLng>>(collection[2]);
    assert(poly->exterior.back()==poly->exterior.front()); // enclosed
    assert(poly->exterior.at(0)==LngLat(60.79057523088477,65.15584934053905));
    assert(poly->exterior.at(1)==LngLat(67.82182523088474,54.585487558200384));
    // ...
    assert(poly->interiors.empty());
    // ===================================================

    // POLYGON WITH INTERIORS ============================
    auto poly_i = check_geometry<Polygon<LatLng>>(collection[3]);
    assert(poly_i->exterior.size() == 5);
    assert(poly_i->interiors.size() == 2);
    assert(poly_i->interiors[0].size() == 4);
    // ===================================================

    // MULTIPOINT ========================================
    auto multipoint = check_geometry<MultiPoint<LatLng>>(collection[4]);
    assert(multipoint->size()==3);
    assert(multipoint->at(0).coordinates==LngLat(35.000589204375075,50.12036070103687));
    // ===================================================

    // MULTILINESTRING ===================================
    auto multilinestring = check_geometry<MultiLineString<LatLng>>(collection[5]);
    assert(multilinestring->size() == 3);
    assert(multilinestring->at(0).size() == 3);
    assert(multilinestring->at(1)[1]==LngLat( 47.72769267664964,48.74573238099538));
    // ===================================================

    // MULTIPOLYGON ======================================
    auto multipolygon = check_geometry<MultiPolygon<LatLng>>(collection[6]);
    assert(multipolygon->size() == 2);
    assert(multipolygon->at(0).exterior.size() == 5);
    assert(multipolygon->at(0).interiors.size() == 0);
    assert(multipolygon->at(0).exterior[1] == LngLat(61.26331128171543,46.35275950149696));
    assert(multipolygon->at(1).exterior.size() == 5);
    assert(multipolygon->at(1).interiors.size() == 0);
    assert(multipolygon->at(1).exterior[1] == LngLat(68.90267539590556,46.4598254513661));
    // ===================================================

    // MULTIPOLYGON WITH INTERIORS =======================
    auto multipolygon_i = check_geometry<MultiPolygon<LatLng>>(collection[7]);
    assert(multipolygon_i->size() == 2);
    assert(multipolygon_i->at(0).exterior.size() == 5);
    assert(multipolygon_i->at(0).interiors.size() == 2);
    assert(multipolygon_i->at(0).exterior[1] == LngLat(83.09838491903668,46.50128452282573));
    assert(multipolygon_i->at(0).interiors[0][1] == LngLat( 87.08533904228551,51.9454930221365));
    assert(multipolygon_i->at(1).exterior.size() == 5);
    assert(multipolygon_i->at(1).interiors.size() == 2);
    assert(multipolygon_i->at(1).exterior[1] == LngLat( 91.55735672152088,46.610949726035386));
    assert(multipolygon_i->at(1).interiors[0][1] == LngLat( 94.68994057384083,52.03861841054476));
    // ===================================================
}

void test_serialize(){
    const QString geojson_test_case = "all_types";

    QString geojson_file_in = geojson_test_case + ".json";
    QString geojson_file_out = geojson_test_case + "_back.json";

    QJsonDocument doc = readJson(geojson_file_in);
    QVariantMap map = doc.toVariant().toMap();

    IFeature *feature = GeoJsonProvider::transformIFeature(map);
    map = GeoJsonProvider::transformIFeature(feature);
    doc = QJsonDocument::fromVariant(map);
    writeJson(geojson_file_out, doc);
}

template <typename T>
void check_property(const Feature::Properties &props, std::string key, T value){
    auto var_value = props.at(key);
    assert(std::holds_alternative<T>(var_value));
    assert(std::get<T>(var_value) == value);
}

void test_base_properties_types(Feature::Properties &props){
    check_property(props, "str_val",std::string("string"));
    check_property(props,"bool_val",false);
    check_property(props,"double_val",0.314);
    check_property(props,"int_val",7);
};

void test_properties_quickaccess(Feature::Properties &props){
    assert(props.has("int_val"));
    assert(!props.has("sdsadsdadsadsad"));

    assert(props["double_val"].is<double>());
    assert(!props["str_val"].is<double>());

    assert(props["str_val"]==std::string("string"));
    assert(props["bool_val"]==false);
    assert(props["double_val"]==0.314);
    assert(props["int_val"]==7);
}

void test_properties_subnodes(Feature::Properties &props){
    assert(props.has("node_val"));

    auto sub_node1 = props["node_val"].to<Feature::Properties>();
    assert(sub_node1["node_num"]==1);

    auto sub_node2 = sub_node1["node_val"].to<Feature::Properties>();
    assert(sub_node2["node_num"]==2);

    auto sub_node3 = sub_node2["node_val"].to<FeatureProperties>();
    assert(sub_node3["node_num"]==3);
}

void test_properties(){
    QJsonDocument doc = readJson("test_properties.json");
    QVariantMap map = doc.toVariant().toMap();

    FeatureCollection collection = GeoJsonProvider::transformCollection(map);
    Feature *feature = dynamic_cast<Feature*>(collection[0]);
    assert(feature->properties.size() == 5);

    test_base_properties_types(feature->properties);
    test_properties_quickaccess(feature->properties);
    test_properties_subnodes(feature->properties);
}


int main(int argc, char *argv[]){
    test_types();
    test_serialize();
    test_properties();

    return 0;
}